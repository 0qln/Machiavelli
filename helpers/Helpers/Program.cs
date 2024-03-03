using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Text;
using System.Text.RegularExpressions;


internal class Program
{
    private static async Task Main(string[] args)
    {
        var Engines = File.ReadAllLines(@"../../../InputData/PerftErrorTrackDown/enginePaths.txt");

        await PerftErrorTrackDown(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 
            6,
            Engines[0],
            Engines[1], 
            x => x.Contains("Stockfish"));

        Console.Read();
    }

    private static void PsqtExtract(string data)
    {
        var PSQTs = Regex.Matches(data, @"(?<=\{)([^\}\{])+(?=\})").Select(x => x.Value.Split("\n")[1..^1]);
        foreach (var psqt in PSQTs)
        {
            PsqtTurn(psqt);
            foreach (var line in psqt)
            {
                Console.WriteLine(line);
            }
            Console.WriteLine();
        }
    } 

    private static void PsqtTurn(string[] lines)
    {
        if (lines.Length != 8)
            throw new ArgumentException();

        Array.Reverse(lines);
    }

    private static async Task PerftErrorTrackDown(string fen, int depth, string testEngine, string controlEngine, Predicate<string> noiseFilter)
    {
        Console.WriteLine("Begin Perft error track down...");
        Console.WriteLine($"Engine 1: {testEngine}");
        Console.WriteLine($"Engine 2: {controlEngine}");

        List<string> moves = new();
        PerftError lastError = default;

        string getPositionCommand() => $"position fen {fen} {(moves.Any() ? "moves " + moves.Aggregate((x, y) => x + " " + y) : string.Empty)}";
        string getPerftCommand(int depth) => $"go perft {depth}";

        while (true)
        {
            StringBuilder current = new(), expected = new();

            string position = getPositionCommand();
            Console.WriteLine($"Searching position: {position}");

            Task 
                result1Task = ExecuteEnginePerft( 
                    testEngine,
                    position,
                    getPerftCommand(depth),
                    current, 
                    noiseFilter), 

                result2Task = ExecuteEnginePerft(
                    controlEngine,
                    position,
                    getPerftCommand(depth),
                    expected,
                    noiseFilter);

            // Wait for both instances to complete
            await Task.WhenAll(result1Task, result2Task);

            // Process error
            Console.WriteLine("Perft outputs: \n");
            Console.WriteLine("Engine 1: ");
            Console.WriteLine(current);
            Console.WriteLine("Engine 2: ");
            Console.WriteLine(expected);
            lastError = GetPerftErrors(current.ToString(), expected.ToString()).FirstOrDefault();

            if (--depth < 1
                || lastError.Expected is null
                || lastError.Current is null)
            {
                break;
            }

            if (lastError == default)
            {
                Console.WriteLine("No errors detected.");
                break;
            }
            else
            {
                moves.Add(lastError.Current?.move ?? lastError.Expected?.move ?? throw new ArgumentException("An empty error was received."));
                Console.WriteLine($"Selected error: {lastError}");
                Console.WriteLine($"Folllowing move: {moves.Last()}");
            }
        }

        Console.WriteLine("\nPerft error track down results:");
        Console.WriteLine($"Last error: \"{lastError}\"");
        Console.WriteLine($"In position: \"{getPositionCommand()}\"");
    }

    private static bool IsEndOfPerft(string? s) => s is not null &&
        (// Custom notation
        Regex.Match(s, @"Nodes searched: .+").Success ||
        // UCI notation
        Regex.Match(s, @"info nodes .+ time .+ nps").Success);

    private static async Task ExecuteEnginePerft(string engine, string position, string perft, StringBuilder resultStream, Predicate<string> noiseFilter)
    {
        // Create a new process start info for PowerShell
        ProcessStartInfo psi;

        if (engine.EndsWith(".exe", StringComparison.OrdinalIgnoreCase))
        {
            // If the engine is an .exe file
            psi = new ProcessStartInfo
            {
                FileName = engine,
                RedirectStandardInput = true,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                CreateNoWindow = false,
            };
        }
        else if (engine.EndsWith(".jar", StringComparison.OrdinalIgnoreCase))
        {
            // If the engine is a .jar file
            psi = new ProcessStartInfo
            {
                FileName = "powershell.exe",
                Arguments = $"java -jar {engine}",
                RedirectStandardInput = true,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                CreateNoWindow = false,
            };
        }
        else
        {
            throw new ArgumentException("Unsupported engine type. Only .exe and .jar files are supported.");
        }

        // Start the process
        using (Process powershellProcess = new Process { StartInfo = psi })
        {
            // Start engine
            powershellProcess.Start();

            powershellProcess.StandardInput.WriteLine(position);

            powershellProcess.StandardInput.WriteLine(perft);

            string? output;
            do
            {
                output = await powershellProcess.StandardOutput.ReadLineAsync();
                if (string.IsNullOrWhiteSpace(output) || noiseFilter(output)) continue;
                resultStream.AppendLine(output);
            }
            while (!IsEndOfPerft(output));

            powershellProcess.StandardInput.WriteLine("quit");

            await powershellProcess.WaitForExitAsync();
        }
    }

    private static Dictionary<string, int> ProcessData(string data)
    {
        var (rName, rCount) = ("", "");

        if (data.Contains("info"))
        {
            // UCI-formatted 
            // <info currmove b1c3 nodes 20>
            rName = @"(?<=currmove )\S+";
            rCount = @"(?<=nodes )\d+";
        }
        else
        {
            // Custom-formatted, probably stockfish notation
            // <b1c3: 20>
            rName = @"^[a-z1-8]+";
            rCount = @"(?<=\: )[0-9]+";
        }

        return data
            .Split("\n")
            .Where(x => !string.IsNullOrWhiteSpace(x))
            .Select(x => new KeyValuePair<string, int>(
                key: Regex.Match(x, rName).Value,
                value: int.Parse(Regex.Match(x, rCount).Value)
            ))
            .ToDictionary();
    }

    private static IEnumerable<PerftError> GetPerftErrors(string current, string expected)
    {
        Console.WriteLine("Perft comparison");

        var (processed1, processed2) = (ProcessData(current), ProcessData(expected));

        // Compare the key existences
        foreach (var kvp in processed1)
        {
            if (!processed2.ContainsKey(kvp.Key))
            {
                Console.WriteLine($"Key of {kvp} in `current` was not found in `expected`.");
                yield return new PerftError { Current = new(kvp.Key, kvp.Value), Expected = null };
            }
        }
        foreach (var kvp in processed2)
        {
            if (!processed1.ContainsKey(kvp.Key))
            {
                Console.WriteLine($"Key of {kvp} in `expected` was not found in `current`.");
                yield return new PerftError { Current = null, Expected = new(kvp.Key, kvp.Value) };
            }
        }

        // Compare values 
        int deltas = 0;
        foreach (var kvp in processed1)
        {
            if (processed2.TryGetValue(kvp.Key, out int value2))
            {
                if (kvp.Value != value2)
                {
                    deltas++;
                    var v1 = processed1[kvp.Key];
                    var v2 = processed2[kvp.Key];
                    Console.WriteLine($"Value difference at key {kvp.Key} [ `current`: {v1}, `expected`: {v2} , delta: {v1 - v2}]");
                    yield return new PerftError { Current = new(kvp.Key, v1), Expected = new(kvp.Key, v2) };
                }
            }
        }

        Console.WriteLine($"Detected {deltas}/{processed2.Count} value differences.");
    }

    private static IEnumerable<PerftError> GetPerftErrors()
    {
        var (current, expected) = (
            File.ReadAllText(@"../../../InputData/PerftComparison/current.txt"), 
            File.ReadAllText(@"../../../InputData/PerftComparison/expected.txt")
        );

        return GetPerftErrors(current, expected);
    }

    record struct PerftError(PerftIndex? Current, PerftIndex? Expected);
    record struct PerftIndex(string move, int count);

    private static void PerftComparison()
    {
        var (current, expected) = (File.ReadAllText(@"../../../InputData/PerftComparison/current.txt"), File.ReadAllText(@"../../../InputData/PerftComparison/expected.txt"));

        Dictionary<string, int> ProcessData(string data)
        {
            var (rName, rCount) = ("", "");

            if (data.Contains("info"))
            {
                // UCI-formatted 
                // <info currmove b1c3 nodes 20>
                rName = @"(?<=currmove )\S+";
                rCount = @"(?<=nodes )\d+";
            }
            else
            {
                // Custom-formatted, probably stockfish notation
                // <b1c3: 20>
                rName = @"^[a-z1-8]+";
                rCount = @"(?<=\: )[0-9]+";
            }

            return data
                .Split("\n")
                .Select(x => new KeyValuePair<string, int>(
                    key: Regex.Match(x, rName).Value,
                    value: int.Parse(Regex.Match(x, rCount).Value)
                ))
                .ToDictionary();
        }

        Console.WriteLine("PERFT COMPARISON");

        var (processed1, processed2) = (ProcessData(current), ProcessData(expected));

        // Compare the key existances
        foreach (var kvp in processed1)
        {
            if (!processed2.ContainsKey(kvp.Key))
            {
                Console.WriteLine($"Key of {kvp} in `current` was not found in `expected`.");
            }
        }
        foreach (var kvp in processed2)
        {
            if (!processed1.ContainsKey(kvp.Key))
            {
                Console.WriteLine($"Key of {kvp} in `expected` was not found in `current`.");
            }
        }

        // Compare values 
        int deltas = 0;
        foreach (var kvp in processed1)
        {
            if (processed2.TryGetValue(kvp.Key, out int value2))
            {
                if (kvp.Value != value2)
                {
                    deltas++;
                    var v1 = processed1[kvp.Key];
                    var v2 = processed2[kvp.Key];
                    Console.WriteLine($"Value difference at key {kvp.Key} [ `current`: {v1}, `expected`: {v2} , delta: {v1 - v2}]");
                }
                else
                {
                    Console.WriteLine($"No value difference for key {kvp.Key}.");
                }
            }
        }
        Console.WriteLine($"Detected {deltas}/{processed2.Count} value differences.");
    }
}
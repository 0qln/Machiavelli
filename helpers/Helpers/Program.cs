using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Text.RegularExpressions;


internal class Program
{
    private static async Task Main(string[] args)
    {
        var Engines = File.ReadAllLines(@"../../../InputData/PerftErrorTrackDown/enginePaths.txt");

        await PerftErrorTrackDown(
            "startposition", 
            4,
            Engines[0],
            Engines[1]);

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

    private static async Task PerftErrorTrackDown(string fen, int depth, string engine1, string engine2)
    {
        List<string> moves = new();

        while (depth >= 1)
        {
            Task result1Task = ExecuteEngineInstance( 
                engine1, 
                [ 
                    $"position {fen} moves {(moves.Any() ? moves.Aggregate((x, y) => x + " " + y) : string.Empty)}",
                    $"go perft {depth}",
                ], 
                @"../../../InputData/PerftComparison/current.txt");
            Task result2Task = ExecuteEngineInstance(
                engine2,
                [
                    $"position {fen} moves {(moves.Any() ? moves.Aggregate((x, y) => x + " " + y) : string.Empty)}",
                    $"go perft {depth}",
                ], 
                @"../../../InputData/PerftComparison/expected.txt");

            // Wait for both instances to complete
            await Task.WhenAll(result1Task, result2Task);
            
            // Process error
            var error = GetPerftErrors().FirstOrDefault();
            if (error == default)
            {
                break;
            }
            else
            {
                moves.Add(error.Current?.move ?? error.Expected?.move ?? throw new ArgumentException("An empty error was received."));
                Console.WriteLine($"Selected error: {error}");
                Console.WriteLine($"Folllowing move: {moves.Last()}");
            }
        }
    }

    private static async Task ExecuteEngineInstance(string engine, IEnumerable<string> commands, string outputPath)
    {
        // Create a new process start info for PowerShell
        ProcessStartInfo psi = new ProcessStartInfo
        {
            FileName = engine,
            RedirectStandardInput = true,
            RedirectStandardOutput = true,
            RedirectStandardError = true,
            UseShellExecute = false,
            CreateNoWindow = false,
        };

        // Start the process
        using (Process powershellProcess = new Process { StartInfo = psi })
        {
            // Start engine
            powershellProcess.Start();

            // Forward commands
            foreach (var command in commands)
            {
                powershellProcess.StandardInput.WriteLine(command);
            }
            
            // Fait for computation to finisih
            Thread.Sleep(2000);
            
            // Close engine
            powershellProcess.StandardInput.WriteLine("quit");

            // Read the output
            string output = await powershellProcess.StandardOutput.ReadToEndAsync();
            string error = await powershellProcess.StandardError.ReadToEndAsync();

            // Wait for the PowerShell instance to exit
            await powershellProcess.WaitForExitAsync();

            // Handle error from engine
            if (!string.IsNullOrEmpty(error))
            {
                Console.WriteLine($"Error: {error}");
                throw new Exception(error);
            }
            // Save the output
            else
            {
                File.WriteAllText(outputPath, output);
            }
        }
    }

    private static IEnumerable<PerftError> GetPerftErrors()
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
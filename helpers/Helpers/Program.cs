


using System.Text.RegularExpressions;

internal class Program
{
    private static void Main(string[] args)
    {
        PerftComparison();
        Console.Read();
    }

    private static void PerftComparison()
    {
        var current = File.ReadAllText(@"../../../InputData/PerftComparison/current.txt");
        var expected = File.ReadAllText(@"../../../InputData/PerftComparison/expected.txt");

        Dictionary<string, int> ProcessData(string data)
        {
            return data
                .Split("\n")
                .Select(x => new KeyValuePair<string, int>(
                    key: Regex.Match(x, @"^[a-z1-8]+").Value,
                    value: int.Parse(Regex.Match(x, @"(?<=\: )[0-9]+").Value)
                ))
                .ToDictionary();
        }

        Console.WriteLine("PERFT COMPARISON");

        var processed1 = ProcessData(current);
        var processed2 = ProcessData(expected);

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
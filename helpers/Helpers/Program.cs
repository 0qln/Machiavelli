


using System.Text.RegularExpressions;

internal class Program
{
    private static void Main(string[] args)
    {
        PerftComparison(
@"h3g2-12: 1
b4b3-0 : 1
b4c3-12: 1
a6e2-12: 1
a6d3-0 : 1
a6c4-0 : 1
a6b5-0 : 1
a6b7-0 : 1
a6c8-0 : 1
b6c8-0 : 1
b6a4-0 : 1
b6c4-0 : 1
b6d5-12: 1
e6e5-0 : 1
e6d5-12: 1
f6g8-0 : 1
f6h7-0 : 1
f6e4-12: 1
f6g4-0 : 1
f6d5-12: 1
f6h5-0 : 1
g6g5-0 : 1
c7c6-0 : 1
c7c5-1 : 1
d7d6-0 : 1
e7c5-0 : 1
e7d6-0 : 1
e7d8-0 : 1
e7f8-0 : 1
e7f7-12: 1
g7h6-0 : 1
g7f8-0 : 1
a8b8-0 : 1
a8c8-0 : 1
a8d8-0 : 1
e8f8-0 : 1
e8f7-12: 1
e8g8-10: 1
e8c8-11: 1
h8h4-0 : 1
h8h5-0 : 1
h8h6-0 : 1
h8h7-0 : 1
h8f8-0 : 1
h8g8-0 : 1",

@"b4b3: 1
e6e5: 1
g6g5: 1
c7c6: 1
d7d6: 1
c7c5: 1
h3g2: 1
e6d5: 1
b4c3: 1
b6a4: 1
b6c4: 1
b6d5: 1
b6c8: 1
f6e4: 1
f6g4: 1
f6d5: 1
f6h5: 1
f6h7: 1
f6g8: 1
a6e2: 1
a6d3: 1
a6c4: 1
a6b5: 1
a6b7: 1
a6c8: 1
g7h6: 1
g7f8: 1
a8b8: 1
a8c8: 1
a8d8: 1
h8h4: 1
h8h5: 1
h8h6: 1
h8h7: 1
h8f8: 1
h8g8: 1
e7c5: 1
e7d6: 1
e7f7: 1
e7d8: 1
e7f8: 1
e8f7: 1
e8g8: 1
e8f8: 1");
    }

    private static void PerftComparison(string current, string expected)
    {
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
        foreach (var kvp in processed1)
        {
            if (processed2.TryGetValue(kvp.Key, out int value2))
            {
                if (kvp.Value != value2)
                {
                    Console.WriteLine($"Value difference at key {kvp.Key} [ `current`: {processed1[kvp.Key]}, `expected`: {processed2[kvp.Key]} ]");
                }
            }
        }
    }
}
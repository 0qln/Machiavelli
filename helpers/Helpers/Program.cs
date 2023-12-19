


using System.Text.RegularExpressions;

internal class Program
{
    private static void Main(string[] args)
    {
        PerftComparison(
@"a1b1-0 : 42
a1c1-0 : 42
a1d1-0 : 42
e1d1-0 : 42
e1f1-0 : 42
e1g1-10: 42
e1c1-11: 42
h1f1-0 : 42
h1g1-0 : 42
a2a3-0 : 43
a2a4-1 : 43
b2b3-0 : 41
d2c1-0 : 42
d2e3-0 : 42
d2f4-0 : 42
d2g5-0 : 41
d2h6-0 : 40
e2d1-0 : 43
e2f1-0 : 43
e2d3-0 : 41
e2c4-0 : 40
e2b5-0 : 38
e2a6-12: 35
g2g3-0 : 41
g2g4-1 : 41
g2h3-12: 42
c3b5-0 : 38
c3a4-0 : 41
c3b1-0 : 41
c3d1-0 : 41
f3g4-0 : 42
f3h5-0 : 42
f3d3-0 : 41
f3e3-0 : 42
f3g3-0 : 42
f3h3-12: 42
f3f4-0 : 42
f3f5-0 : 44
f3f6-12: 38
d5d6-0 : 40
d5e6-12: 45
e5f7-12: 43
e5d7-12: 44
e5g6-12: 42
e5c6-0 : 40
e5d3-0 : 42
e5c4-0 : 41
e5g4-0 : 43",

@"a2a3: 44
b2b3: 42
g2g3: 42
d5d6: 41
a2a4: 44
g2g4: 42
g2h3: 43
d5e6: 46
c3b1: 42
c3d1: 42
c3a4: 42
c3b5: 39
e5d3: 43
e5c4: 42
e5g4: 44
e5c6: 41
e5g6: 42
e5d7: 45
e5f7: 44
d2c1: 43
d2e3: 43
d2f4: 43
d2g5: 42
d2h6: 41
e2d1: 44
e2f1: 44
e2d3: 42
e2c4: 41
e2b5: 39
e2a6: 36
a1b1: 43
a1c1: 43
a1d1: 43
h1f1: 43
h1g1: 43
f3d3: 42
f3e3: 43
f3g3: 43
f3h3: 43
f3f4: 43
f3g4: 43
f3f5: 45
f3h5: 43
f3f6: 39
e1d1: 43
e1f1: 43
e1g1: 43
e1c1: 43");
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
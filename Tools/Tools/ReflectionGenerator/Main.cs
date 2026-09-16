using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Xml.Linq;

public class Program
{
    public static void Main(string[] args)
    {


        if(args.Length == 0)
        {
            // Run local 
        }






        const string fieldRegex = @"\b(?:class|struct)\s+(?:[A-Za-z_][A-Za-z0-9_]*\s+)?([A-Za-z_][A-Za-z0-9_]*)\s*([\s\S]*)";

        string test = File.ReadAllText("C:/Users/zulto/Desktop/MyFiles/Projects/Project-Nova/Project/Assets/PlayerMovement.h");
        var match = Regex.Match(test, fieldRegex);
    }
}
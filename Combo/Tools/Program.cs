using System;
using Combo.Tools;
using System.Reflection;
using System.Linq;
using System.Collections.Generic;

namespace Combo.Tools {
    public class Program {
        public static Dictionary<String,Combo.BuildTools.BuildTarget> targets;
        public static void Main(string[] args) {
            ArgumentParser parser = new ArgumentParser();
            Options options = parser.accept<Options>(args);   


            switch(options.type) {
                case "build":
                    targets = BuildTargetAssembler.generateAssembliesFromTargets();

                    if(options.name == null) {
                        Console.WriteLine("Building all...");

                        foreach(KeyValuePair<string,Combo.BuildTools.BuildTarget> entry in targets) {
                            Console.WriteLine("Building " + entry.Value.GetName());
                            Compiler.CompileElement(entry.Key,entry.Value);
                        }
                    } else {
                        Combo.BuildTools.BuildTarget finalTarget = null;
                        string path = null;
                        foreach(KeyValuePair<string,Combo.BuildTools.BuildTarget> entry in targets) {
                            if(string.Equals(entry.Value.GetName(), options.name, StringComparison.OrdinalIgnoreCase)) {
                                finalTarget = entry.Value;
                                path = entry.Key;
                            }
                        }

                        if(finalTarget == null) {
                            Console.WriteLine("ERROR: Unable to find target with this name");
                            return;
                        }

                        Console.WriteLine("Building " + finalTarget.GetName() + "");
                    }
                    break;
                case "projects":
                    targets = BuildTargetAssembler.generateAssembliesFromTargets();
                    PremakeScriptBuilder.createPremake5Scripts(targets);
                    Console.WriteLine("Generated projects");
                    break;
                case "setup":
                    if(options.name != null) {
                        ComboProjectGenerator.Setup(options.name);
                        Console.WriteLine("Generated project " + options.name);
                    } else {
                        Console.WriteLine("Name is required argument!");
                    }
                    break;

            }
        }
    }
}
using System;
using Astur.Tools;
using Microsoft.CSharp;
using System.CodeDom.Compiler;

namespace Astur.Tools {
    public class Program {
        public static void Main(string[] args) {
            ArgumentParser parser = new ArgumentParser();
            Options options = parser.accept<Options>(args);
            
            Console.WriteLine(options.action);
        }
    }
}
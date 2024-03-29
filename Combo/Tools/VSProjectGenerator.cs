using System;
using System.Collections.Generic;
using Combo.BuildTools;
using System.Text;
using System.Xml;
using System.IO;

namespace Combo.Tools {
    public class PremakeScriptBuilder {
        public static void createPremake5Scripts(Dictionary<string,BuildTarget> targets) {
            foreach(KeyValuePair<string, BuildTarget> entry in targets)
            {
                if(entry.Value.GetLanguage() == Language.Cpp) {
                    if(entry.Value.GetBuildType() == BuildType.Project) {
                        continue;
                    }
                    String xmlPath = Path.GetDirectoryName(entry.Key) + "\\" + entry.Value.GetName() + ".vcxproj";
                    XmlDocument doc = new XmlDocument();
                    XmlDeclaration declaration = doc.CreateXmlDeclaration("1.0", "utf-8", null);
                    doc.AppendChild(declaration);

                

                    XmlElement projectElement = doc.CreateElement("Project");
                    projectElement.SetAttribute("DefaultTargets", "Build");
                    projectElement.SetAttribute("ToolsVersion", "4.0");
                    projectElement.SetAttribute("xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");
                    doc.AppendChild(projectElement);

                    XmlElement itemGroupElement = doc.CreateElement("ItemGroup");
                    itemGroupElement.SetAttribute("Label", "ProjectConfigurations");
                    projectElement.AppendChild(itemGroupElement);

                    XmlElement projectConfig1Element = doc.CreateElement("ProjectConfiguration");
                    projectConfig1Element.SetAttribute("Include", "Debug|x64");
                    itemGroupElement.AppendChild(projectConfig1Element);

                    XmlElement config1Element = doc.CreateElement("Configuration");
                    config1Element.InnerText = "Debug";
                    projectConfig1Element.AppendChild(config1Element);

                    XmlElement platform1Element = doc.CreateElement("Platform");
                    platform1Element.InnerText = "x64";
                    projectConfig1Element.AppendChild(platform1Element);

                    XmlElement projectConfig2Element = doc.CreateElement("ProjectConfiguration");
                    projectConfig2Element.SetAttribute("Include", "Release|x64");
                    itemGroupElement.AppendChild(projectConfig2Element);

                    XmlElement config2Element = doc.CreateElement("Configuration");
                    config2Element.InnerText = "Release";
                    projectConfig2Element.AppendChild(config2Element);

                    XmlElement platform2Element = doc.CreateElement("Platform");
                    platform2Element.InnerText = "x64";
                    projectConfig2Element.AppendChild(platform2Element);

                    XmlElement propGroup1Element = doc.CreateElement("PropertyGroup");
                    propGroup1Element.SetAttribute("Label", "Globals");
                    projectElement.AppendChild(propGroup1Element);

                    XmlElement projectGuidElement = doc.CreateElement("ProjectGuid");
                    projectGuidElement.InnerText = "{96F21549-A7BF-4695-A1B1-B43625B91A14}";
                    propGroup1Element.AppendChild(projectGuidElement);

                    XmlElement keywordElement = doc.CreateElement("Keyword");
                    keywordElement.InnerText = "x64Proj";
                    propGroup1Element.AppendChild(keywordElement);

                    XmlElement rootNamespaceElement = doc.CreateElement("RootNamespace");
                    rootNamespaceElement.InnerText = entry.Value.GetName();
                    propGroup1Element.AppendChild(rootNamespaceElement);

                    XmlElement import1Element = doc.CreateElement("Import");
                    import1Element.SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.Default.props");
                    projectElement.AppendChild(import1Element);

                    XmlElement propGroup2Element = doc.CreateElement("PropertyGroup");
                    propGroup2Element.SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Debug|x64'");
                    propGroup2Element.SetAttribute("Label", "Configuration");
                    projectElement.AppendChild(propGroup2Element);


                    XmlElement charSet1Element = doc.CreateElement("CharacterSet");
                    charSet1Element.InnerText = "Unicode";
                    propGroup2Element.AppendChild(charSet1Element);

                    XmlElement outDir1 = doc.CreateElement("OutDir");
                    XmlElement intDir1 = doc.CreateElement("IntDir");
                    outDir1.InnerText = "Binary";
                    intDir1.InnerText = "Binary\\int";

                    propGroup2Element.AppendChild(outDir1);
                    propGroup2Element.AppendChild(intDir1);

                    XmlElement buildType1 = doc.CreateElement("ConfigurationType");
                    buildType1.InnerText = entry.Value.GetBuildType() == BuildType.Executable ? "Application" : "DynamicLibrary";
                    propGroup2Element.AppendChild(buildType1);

                    XmlElement propGroup3Element = doc.CreateElement("PropertyGroup");
                    propGroup3Element.SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Release|x64'");
                    propGroup3Element.SetAttribute("Label", "Configuration");
                    projectElement.AppendChild(propGroup3Element);

                    XmlElement wholeProgramOptElement = doc.CreateElement("WholeProgramOptimization");
                    wholeProgramOptElement.InnerText = "true";
                    propGroup3Element.AppendChild(wholeProgramOptElement);

                    XmlElement charSet2Element = doc.CreateElement("CharacterSet");
                    charSet2Element.InnerText = "Unicode";
                    propGroup3Element.AppendChild(charSet2Element);


                    XmlElement outDir2 = doc.CreateElement("OutDir");
                    XmlElement intDir2 = doc.CreateElement("IntDir");
                    outDir2.InnerText = "Binary";
                    intDir2.InnerText = "Binary\\int";

                    propGroup3Element.AppendChild(outDir2);
                    propGroup3Element.AppendChild(intDir2);

                    XmlElement buildType2 = doc.CreateElement("ConfigurationType");
                    buildType2.InnerText = entry.Value.GetBuildType() == BuildType.Executable ? "Application" : "DynamicLibrary";
                    propGroup3Element.AppendChild(buildType2);


                    XmlElement import2Element = doc.CreateElement("Import");
                    import2Element.SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.props");
                    projectElement.AppendChild(import2Element);

                    XmlElement importGroup1Element = doc.CreateElement("ImportGroup");
                    importGroup1Element.SetAttribute("Label", "ExtensionSettings");
                    projectElement.AppendChild(importGroup1Element);

                    XmlElement importGroup2Element = doc.CreateElement("ImportGroup");
                    importGroup2Element.SetAttribute("Label", "PropertySheets");
                    importGroup2Element.SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Debug|x64'");
                    projectElement.AppendChild(importGroup2Element);

                    XmlElement import2ConditionElement = doc.CreateElement("Import");
                    import2ConditionElement.SetAttribute("Project", "$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props");
                    import2ConditionElement.SetAttribute("Condition", "exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')");
                    import2ConditionElement.SetAttribute("Label", "LocalAppDataPlatform");
                    importGroup2Element.AppendChild(import2ConditionElement);

                    XmlElement importGroup3Element = doc.CreateElement("ImportGroup");
                    importGroup3Element.SetAttribute("Label", "PropertySheets");
                    importGroup3Element.SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Release|x64'");
                    projectElement.AppendChild(importGroup3Element);

                    XmlElement import3ConditionElement = doc.CreateElement("Import");
                    import3ConditionElement.SetAttribute("Project", "$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props");
                    import3ConditionElement.SetAttribute("Condition", "exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')");
                    import3ConditionElement.SetAttribute("Label", "LocalAppDataPlatform");
                    importGroup3Element.AppendChild(import3ConditionElement);

                    XmlElement propGroup4Element = doc.CreateElement("PropertyGroup");
                    propGroup4Element.SetAttribute("Label", "UserMacros");
                    projectElement.AppendChild(propGroup4Element);

                    XmlElement propGroup5Element = doc.CreateElement("PropertyGroup");
                    propGroup5Element.SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Debug|x64'");
                    projectElement.AppendChild(propGroup5Element);

                    XmlElement linkIncremental1Element = doc.CreateElement("LinkIncremental");
                    linkIncremental1Element.InnerText = "true";
                    propGroup5Element.AppendChild(linkIncremental1Element);

                    XmlElement libraryPath = doc.CreateElement("LibraryPath");
                    libraryPath.InnerText = string.Join(";",entry.Value.GetLibraryDirectories()) + ";$(LibraryPath)";
                    propGroup5Element.AppendChild(libraryPath);

                    XmlElement includePath = doc.CreateElement("IncludePath");
                    includePath.InnerText = string.Join(";",entry.Value.GetIncludeDirectories()) + ";$(IncludePath)";
                    propGroup5Element.AppendChild(includePath);

                    XmlElement propGroup6Element = doc.CreateElement("PropertyGroup");
                    propGroup6Element.SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Release|x64'");
                    projectElement.AppendChild(propGroup6Element);

                    XmlElement linkIncremental2Element = doc.CreateElement("LinkIncremental");
                    linkIncremental2Element.InnerText = "false";
                    propGroup6Element.AppendChild(linkIncremental2Element);

                    XmlElement libraryPath2 = doc.CreateElement("LibraryPath");
                    libraryPath2.InnerText = string.Join(";",entry.Value.GetLibraryDirectories()) + ";$(LibraryPath)";
                    propGroup6Element.AppendChild(libraryPath2);

                    XmlElement includePath2 = doc.CreateElement("IncludePath");
                    includePath2.InnerText = string.Join(";",entry.Value.GetIncludeDirectories()) + ";$(IncludePath)";
                    propGroup6Element.AppendChild(includePath2);

                    XmlElement itemDefGroup1Element = doc.CreateElement("ItemDefinitionGroup");
                    itemDefGroup1Element.SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Debug|x64'");
                    projectElement.AppendChild(itemDefGroup1Element);

                    XmlElement clCompile1Element = doc.CreateElement("ClCompile");
                    itemDefGroup1Element.AppendChild(clCompile1Element);

                    XmlElement warningLevel1Element = doc.CreateElement("WarningLevel");
                    warningLevel1Element.InnerText = "Level3";
                    clCompile1Element.AppendChild(warningLevel1Element);

                    XmlElement minimalRebuildElement = doc.CreateElement("MinimalRebuild");
                    minimalRebuildElement.InnerText = "true";
                    clCompile1Element.AppendChild(minimalRebuildElement);

                    XmlElement debugInfoFormat1Element = doc.CreateElement("DebugInformationFormat");
                    debugInfoFormat1Element.InnerText = "EditAndContinue";
                    clCompile1Element.AppendChild(debugInfoFormat1Element);

                    XmlElement optimization1Element = doc.CreateElement("Optimization");
                    optimization1Element.InnerText = "Disabled";
                    clCompile1Element.AppendChild(optimization1Element);

                    XmlElement basicRuntimeChecksElement = doc.CreateElement("BasicRuntimeChecks");
                    basicRuntimeChecksElement.InnerText = "EnableFastChecks";
                    clCompile1Element.AppendChild(basicRuntimeChecksElement);

                    XmlElement runtimeLibrary1Element = doc.CreateElement("RuntimeLibrary");
                    runtimeLibrary1Element.InnerText = "MultiThreadedDebugDLL";
                    clCompile1Element.AppendChild(runtimeLibrary1Element);

                    XmlElement preprocessorDefs1Element = doc.CreateElement("PreprocessorDefinitions");
                    preprocessorDefs1Element.InnerText = string.Join(";",entry.Value.compilerDefinitions) + ";x64;_DEBUG;_CONSOLE;%(PreprocessorDefinitions)";
                    clCompile1Element.AppendChild(preprocessorDefs1Element);

                    XmlElement standardLang1Element = doc.CreateElement("LanguageStandard");
                    standardLang1Element.InnerText = "stdcpp17";
                    clCompile1Element.AppendChild(standardLang1Element);

                    //PCH
                    if(entry.Value.PCH) {
                        XmlElement precompiledHeader = doc.CreateElement("PrecompiledHeader");
                        precompiledHeader.InnerText = "Use";
                        clCompile1Element.AppendChild(precompiledHeader);
                        

                        XmlElement precompiledHeaderFile = doc.CreateElement("PrecompiledHeaderFile");
                        precompiledHeaderFile.InnerText = "pch.h";
                        clCompile1Element.AppendChild(precompiledHeaderFile);
                    }

                    XmlElement link1Element = doc.CreateElement("Link");
                    itemDefGroup1Element.AppendChild(link1Element);

                    XmlElement subSystem1Element = doc.CreateElement("SubSystem");
                    subSystem1Element.InnerText = "Console";
                    link1Element.AppendChild(subSystem1Element);

                    XmlElement genDebugInfo1Element = doc.CreateElement("GenerateDebugInformation");
                    genDebugInfo1Element.InnerText = "true";
                    link1Element.AppendChild(genDebugInfo1Element);


                    XmlElement itemDefGroup2Element = doc.CreateElement("ItemDefinitionGroup");
                    itemDefGroup2Element.SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Release|x64'");
                    projectElement.AppendChild(itemDefGroup2Element);

                    XmlElement clCompile2Element = doc.CreateElement("ClCompile");
                    itemDefGroup2Element.AppendChild(clCompile2Element);

                    XmlElement warningLevel2Element = doc.CreateElement("WarningLevel");
                    warningLevel2Element.InnerText = "Level3";
                    clCompile2Element.AppendChild(warningLevel2Element);


                    XmlElement debugInfoFormat2Element = doc.CreateElement("DebugInformationFormat");
                    debugInfoFormat2Element.InnerText = "ProgramDatabase";
                    clCompile2Element.AppendChild(debugInfoFormat2Element);

                    XmlElement optimization2Element = doc.CreateElement("Optimization");
                    optimization2Element.InnerText = "MaxSpeed";
                    clCompile2Element.AppendChild(optimization2Element);

                    XmlElement runtimeLibrary2Element = doc.CreateElement("RuntimeLibrary");
                    runtimeLibrary2Element.InnerText = "MultiThreadedDLL";
                    clCompile2Element.AppendChild(runtimeLibrary2Element);

                    XmlElement preprocessorDefs2Element = doc.CreateElement("PreprocessorDefinitions");
                    preprocessorDefs2Element.InnerText = string.Join(";",entry.Value.compilerDefinitions) + "x64;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)";
                    clCompile2Element.AppendChild(preprocessorDefs2Element);

                    XmlElement standardLang2Element = doc.CreateElement("LanguageStandard");
                    standardLang2Element.InnerText = "stdcpp17";
                    clCompile2Element.AppendChild(standardLang2Element);

                    if(entry.Value.PCH) {
                        XmlElement precompiledHeader = doc.CreateElement("PrecompiledHeader");
                        precompiledHeader.InnerText = "Use";
                        clCompile2Element.AppendChild(precompiledHeader);
                        

                        XmlElement precompiledHeaderFile = doc.CreateElement("PrecompiledHeaderFile");
                        precompiledHeaderFile.InnerText = "pch.h";
                        clCompile2Element.AppendChild(precompiledHeaderFile);
                    }

                    XmlElement link2Element = doc.CreateElement("Link");
                    itemDefGroup2Element.AppendChild(link2Element);

                    XmlElement subSystem2Element = doc.CreateElement("SubSystem");
                    subSystem2Element.InnerText = "Console";
                    link2Element.AppendChild(subSystem2Element);

                    XmlElement optimizeReferencesElement = doc.CreateElement("EnableCOMDATFolding");
                    optimizeReferencesElement.InnerText = "true";
                    link2Element.AppendChild(optimizeReferencesElement);

                    XmlElement genDebugInfo2Element = doc.CreateElement("GenerateDebugInformation");
                    genDebugInfo2Element.InnerText = "true";
                    link2Element.AppendChild(genDebugInfo2Element);

                    XmlElement sourceIncludeGroup = doc.CreateElement("ItemGroup");
                    foreach(string path in Directory.GetFiles(Path.GetDirectoryName(entry.Key) + "\\" + entry.Value.GetSourceFolder(), "*.*", SearchOption.AllDirectories)) {
                        if(path.EndsWith(".h") || path.EndsWith(".hpp")) {
                            XmlElement includea = doc.CreateElement("ClInclude");
                            includea.SetAttribute("Include",path.Replace(Path.GetDirectoryName(entry.Key) + "\\",""));
                            sourceIncludeGroup.AppendChild(includea);
                        }
                    }
                    projectElement.AppendChild(sourceIncludeGroup);

                    XmlElement sourceCppGroup = doc.CreateElement("ItemGroup");
                    foreach(string path in Directory.GetFiles(Path.GetDirectoryName(entry.Key) + "\\" + entry.Value.GetSourceFolder(), "*.*", SearchOption.AllDirectories)) {
                        if(path.EndsWith(".cpp") || path.EndsWith(".c")) {
                            XmlElement includea = doc.CreateElement("ClCompile");
                            includea.SetAttribute("Include",path.Replace(Path.GetDirectoryName(entry.Key) + "\\",""));

                            if(entry.Value.PCH && path.EndsWith("pch.cpp")) {
                                XmlElement precompiledHeader = doc.CreateElement("PrecompiledHeader");
                                precompiledHeader.SetAttribute("Condition","'$(Configuration)|$(Platform)'=='Debug|x64'");
                                precompiledHeader.InnerText = "Create";
                                includea.AppendChild(precompiledHeader);

                                XmlElement precompiledHeader2 = doc.CreateElement("PrecompiledHeader");
                                precompiledHeader2.SetAttribute("Condition","'$(Configuration)|$(Platform)'=='Release|x64'");
                                precompiledHeader2.InnerText = "Create";
                                includea.AppendChild(precompiledHeader2);
                            }
                        
                            sourceCppGroup.AppendChild(includea);
                        }
                    }
                    projectElement.AppendChild(sourceCppGroup);

                    XmlElement a =doc.CreateElement("Import");
                    a.SetAttribute("Project",@"$(VCTargetsPath)\Microsoft.Cpp.targets");
                    projectElement.AppendChild(a);


                    XmlElement ImportGroupasd = doc.CreateElement("ImportGroup");
                    ImportGroupasd.SetAttribute("Label","ExtensionTargets");
                    projectElement.AppendChild(ImportGroupasd);


                    doc.Save(xmlPath);
                } else if(entry.Value.GetLanguage() == Language.CSharp) {
                    String xmlPath = Path.GetDirectoryName(entry.Key) + "\\" + entry.Value.GetName() + ".csproj";
                    XmlDocument doc = new XmlDocument();

                    XmlElement projectElement = doc.CreateElement("Project");
                    projectElement.SetAttribute("Sdk", "Microsoft.NET.Sdk");

                    XmlElement propertyGroupElement = doc.CreateElement("PropertyGroup");
                    projectElement.AppendChild(propertyGroupElement);

                    AddPropertyElement(doc, propertyGroupElement, "OutputType", entry.Value.GetBuildType() == Combo.BuildTools.BuildType.Executable ? "Exe" : "Library");
                    AddPropertyElement(doc, propertyGroupElement, "TargetFramework", "net7.0");
                    AddPropertyElement(doc, propertyGroupElement, "ImplicitUsings", "enable");
                    AddPropertyElement(doc, propertyGroupElement, "Nullable", "enable");
                    AddPropertyElement(doc, propertyGroupElement, "AssemblyName", entry.Value.GetName());
                    AddPropertyElement(doc, propertyGroupElement, "PlatformTarget", "x64");
                    AddPropertyElement(doc, propertyGroupElement, "BaseOutputPath", Path.GetFullPath(Path.GetDirectoryName(entry.Key)) + "\\Binary");
                    AddPropertyElement(doc, propertyGroupElement, "BaseIntermediateOutputPath", Path.GetFullPath(Path.GetDirectoryName(entry.Key)) + "\\Binary\\int");
                    AddPropertyElement(doc, propertyGroupElement, "FileVersion", "0.1");
                    AddPropertyElement(doc, propertyGroupElement, "AssemblyVersion", "0.1");

                    doc.AppendChild(projectElement);

                    doc.Save(xmlPath);
                }
            }
        }
        static void AddPropertyElement(XmlDocument doc, XmlElement parentElement, string name, string value)
        {
            XmlElement propertyElement = doc.CreateElement(name);
            propertyElement.InnerText = value;
            parentElement.AppendChild(propertyElement);
        }
    }
}
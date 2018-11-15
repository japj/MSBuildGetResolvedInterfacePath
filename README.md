# MSBuildGetResolvedInterfacePath
Example of extending the Microsoft.CppBuild.targets ProjectReference Protocol for AdditionalIncludeDirectories

## Why
The [ProjectReference Protocol](https://github.com/Microsoft/msbuild/blob/master/documentation/ProjectReference-Protocol.md) 
allows MSBuild and VisualStudio to build & link multiple projects inside a solution.

In short what this allows is for MSBuild to discover what to pass into the compiler & linker.
An clear example of this is that when referencing a DLL from an EXE Project, the DLL Import library
will be 'automatically' linked into the EXE Project.

However, what is currently 'missing' is a way to automatically discover the API headers of the DLL project based on ProjectReferences.

In fact, the [Walkthrough: Create and use your own Dynamic Link Library (C++)](https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=vs-2017#to-add-the-dll-header-to-your-include-path)
has a section on how to manually do this: `To add the DLL header to your include path`.

This example tries to show a way to solve this, using the same mechanism that the `Microsoft.CppBuild.targets` does for resolving reference link libraries.

## TODO ##

* update How section of the documentation
* add sample C++ projects (that can function as a 'unittest' and show the behaviour/working)
* possibly refactor into a nuget package? (t.b.d. if regular C++ projects can use nuget style SDK projects or not)

## How

**TODO**: rework text, the below is a dump of notes I gathered when prototyping and needs to rewritten


    _Link DependsOnTargets $(ComputeLinkInputsTargets)

    ComputeLinkInputsTargets PropertyGroup
      contains ComputeReferenceLinkInputs

    ComputeReferenceLinkInputs
       DependsOnTargets on ResolvedLinkLib
       returns ProjectReferenceToLink

    ResolvedLinkLib
      calls GetResolvedLinkLibs target on _MSBuildProjectReferenceExistent
      TargetOutputs into _ResolvedNativeProjectReferencePaths

    GetResolvedLinkLibs
      uses _ResolvedNativeProjectReferencePaths
      returns calculated LibFullPath


    ---------------------------------------
    _ClCompile DependsOnTargets $(ComputeCompileInputsTargets)

    $(ComputeCompileInputsTargets)
      ComputeMIDLGeneratedCompileInputs
      ComputeCLInputPDBName
      ComputeReferenceCLInput
      WarnCompileDuplicatedFilename

      FixupCLCompileOptions from CppCommon.Targets



    Add target to $(ComputeCompileInputsTargets)
      ComputeReferenceIncludeInputs

    ComputeReferenceIncludeInputs
      DependsOnTargets on ResolvedReferenceIncludeInputs

    ResolvedReferenceIncludeInputs
      calls GetResolvedInterfacePath target on _MSBuildProjectReferenceExistent
      TargetOutputs into _ResolvedProjectReferenceIncludePaths

    GetResolvedInterfacePath
      returns InterfaceFullPath

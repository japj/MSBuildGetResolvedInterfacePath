# MSBuildGetResolvedInterfacePath
Example of extending the Microsoft.CppBuild.targets ProjectReference Protocol for AdditionalIncludeDirectories

## Why
The [ProjectReference Protocol](https://github.com/Microsoft/msbuild/blob/master/documentation/ProjectReference-Protocol.md) 
allows MSBuild and VisualStudio to build & link a project that refers to another project (by means of a `ProjectReference`) .

This `ProjectReference` protocol allows MSBuild to discover what to pass into the linker.
An example of this is that when an Console Application has a `ProjectReference` to a Library project then the relevant `.LIB`
file will be automatically linked into the Console Application.

However, what is currently 'missing' is a way to automatically discover the API headers of the DLL project based on a ProjectReference 
(I will refer to these API headers as the 'interface' header files).

In fact, the official `Walkthrough: Create and use your own Dynamic Link Library (C++)` has a section on how to manually 
add include paths in your project, see: 
[To add the DLL header to your include path](https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=vs-2017#to-add-the-dll-header-to-your-include-path).

This MSBuild example tries to show a way to solve this problem, by automatically adding the 'interface' folder to the compiler include path
using the same mechanism that the `Microsoft.CppBuild.targets` uses for automatically linking in `.LIB` files from `ProjectReferences`.

## How to use ##
In order to use this solution in your own projects, please follow the following convention:

* the public API must reside in a folder called `interface` next to the library project file
* import the `Sdk.Props/Targets` files in both the library project and the 'application'
  * this can be done by a global `Directory.Build.props/targets` files as shown in the example
* make sure you have a `ProjectReference` from the 'application' to your 'library'

That should be enough for it to work.

### Example
The [Example](https://github.com/japj/MSBuildGetResolvedInterfacePath/tree/master/src/Example) 
shows a ConsoleApplication and a Dll (Library) Project.

The Dll projects contains an `interface` folder with the public API (`CMyClass.h`).

The ConsoleApplication's vcxproj contains a `ProjectReference` to this Dll Project:

    <ItemGroup>
      <ProjectReference Include="..\Dll1\Dll1.vcxproj">
        <Project>{714347ed-ecd9-4f68-ab47-d7bda64aa518}</Project>
      </ProjectReference>
    </ItemGroup>

And there is a relevant `Directory.Build.Targets` (and Props file) that imports the Sdk files:

    <?xml version="1.0" encoding="utf-8"?>
    <Project ToolsVersion="15.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">

      <Import Project="..\ResolveInterfaceIncludes\Sdk\Sdk.Targets"/>
  
    </Project>

Futhermore the Console Application uses an `#include "CMyClass.h"` to include 

## How does it work

This implementation is based on the concepts described in the [ProjectReference Protocol](https://github.com/Microsoft/msbuild/blob/master/documentation/ProjectReference-Protocol.md) 
and how `Microsoft.CppBuild.targets` works with `ComputeReferenceLinkInputs`, `ResolvedLinkLib` and `GetResolvedLinkLibs`.

* The `ComputeCompileInputsTargets` is extended with a new `ComputeReferenceIncludeInputs` target

  This allows us to extend the existing behaviour of `Microsoft.CppBuild.targets` during a preparation phase (before compilation actually starts)

* The new `ComputeReferenceIncludeInputs` target will set `ClCompile.AdditionalIncludeDirectories` based on information gathered by `ResolvedReferenceIncludeInputs`

* The new `ResolvedReferenceIncludeInputs` will fill `_ResolvedProjectReferenceIncludePaths` by calling the MSBuild Target `GetResolvedInterfacePath` on each `ProjectReferences` Project.

* The `GetResolvedInterfacePath` will return the full path to the `interface` folder (if it exists)


## Todo Items

* move name of default 'interface' folder into props file, so it can be customized from project file
* investigate [MSBuildSDK](https://github.com/Microsoft/MSBuildSdks) based unittest (using the MSBuild engine?)
* enable correct nuget package? (t.b.d. if regular C++ projects can use nuget style SDK projects or not)

* investigate other solution to enable intellisene (see AlwaysResolveReferencesDesignTime in Sdk.props)
##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=spf
ConfigurationName      :=Debug
WorkspacePath          := "C:\Users\Joe\aframework\workspace"
ProjectPath            := "C:\Users\Joe\aframework\workspace"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Joe
Date                   :=07/01/2016
CodeLitePath           :="C:\Program Files\CodeLite"
LinkerName             :=C:/TDM-GCC-64/bin/g++.exe
SharedObjectLinkerName :=C:/TDM-GCC-64/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="spf.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/TDM-GCC-64/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/TDM-GCC-64/bin/ar.exe rcu
CXX      := C:/TDM-GCC-64/bin/g++.exe
CC       := C:/TDM-GCC-64/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/TDM-GCC-64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/code_main.c$(ObjectSuffix) $(IntermediateDirectory)/code_base.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/code_main.c$(ObjectSuffix): code/main.c $(IntermediateDirectory)/code_main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/Joe/aframework/workspace/code/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/code_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/code_main.c$(DependSuffix): code/main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/code_main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/code_main.c$(DependSuffix) -MM "code/main.c"

$(IntermediateDirectory)/code_main.c$(PreprocessSuffix): code/main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/code_main.c$(PreprocessSuffix) "code/main.c"

$(IntermediateDirectory)/code_base.c$(ObjectSuffix): code/base.c $(IntermediateDirectory)/code_base.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/Joe/aframework/workspace/code/base.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/code_base.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/code_base.c$(DependSuffix): code/base.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/code_base.c$(ObjectSuffix) -MF$(IntermediateDirectory)/code_base.c$(DependSuffix) -MM "code/base.c"

$(IntermediateDirectory)/code_base.c$(PreprocessSuffix): code/base.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/code_base.c$(PreprocessSuffix) "code/base.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/



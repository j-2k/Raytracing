-- premake5.lua
workspace "JumaRaytracing"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "JumaRaytracing"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "JumaRaytracing"
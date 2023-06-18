cmake_minimum_required(VERSION 3.5)
project(DIGUI VERSION 0.1 DESCRIPTION "GUI module" LANGUAGES CXX)


file( GLOB SOURCES **.cpp )

SetFlags()
SetLibraries( DIRenderer DIPhysics DIEvents DIParser )
CreateLibrary( ${PROJECT_NAME} )
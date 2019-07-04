Calculator example amsr-vector-app-example-calculator README
Author: visldm

TOC
----------------------
1. Overview
2. Folder structure
3. Building the example
4. Notes
----------------------

1. Overview
===========

This example implements a basic calculator that communicates over ara::com. There is a calculatorServer that provides a
service called calculatorService. The calculatorInterface contains the following:
- Method add that adds two given arguments
- Method divide that divides two given arguments
- Event divisionByZero signals that a division by zero has occured
- Field divideResult that contains the last valid division result (last division with divident != 0). This field has
  also a notifier, so that the client gets informed when the field changes

####################                                                     ######################
# calculatorClient # uInt32 add(uInt32, uInt32) <method>                 # calculatorServer   #
#                  #<--------------------------------------------------->#                    #
#                  # uInt32 divide(uInt32, uInt32) <method>              #                    #
#                  #<--------------------------------------------------->#                    #
#                  # uInt32 divideResult <field>                         #                    #
#                  #<----------------------------------------------------#                    #
#                  # bool divisionByZero <event>                         #                    #
#                  #<----------------------------------------------------#                    #
####################                                                     ######################


2. Folder structure
===================

"addon"                 contains the calculatorClient
"etc"                   contains the manifest for the Execution Manager. Note: The manifest is only needed when you start the
|  |  |                 applications (client, server, vsomeipd) over the Execution Manager and not manually
"model"                 contains the meta model for the calculator example and the necessary tools
   "tools"              contains necessary tools to generate the ara::com interfaces and the vsomeip configuration
   "arxml"              contains the meta model
"src"                contains the calculatorServer
"src-gen"            contains all generated files for ara::com and the vsomeip binding


3. Building the example
=======================

STEP 1: This repository uses CMake as its build system. You first have to compile and install the vsomeip daemon v2.5.2 from GENIVI (https://github.com/GENIVI/vsomeip).
You have to follow the instructions on this website to compile and install the vsomeipd.

STEP 2: Compile and install the communication-middleware from BMW
SHA1 ID: b4889eae7c3b6e5ffb98a24c5e7b5c668e509c28 / branch: feature/CM-AP-538-integrate-bmw-ara-com from the repository (INFO: Check other README files for further informations):

git clone ssh://<User ID>@review.adaptivemicrosar.vi.vector.int:29418/ascgit166.bmw.paads.fs.communicationmiddleware
git checkout  -b feature/CM-AP-538-integrate-bmw-ara-com e796c59006be44b1e68bc10c7681e96fefe7d67f
mkdir <build folder>
cd <build folder>
cmake <source folder>
make
sudo make install

STEP: 3: Checkout and build the calculator example from repository:
git clone ssh://<User ID>@review.adaptivemicrosar.vi.vector.int:29418/amsr-vector-app-example-calculator
mkdir <build folder>
cd <build folder>
cmake <source folder>
make
sudo make install

Three executables will be built:
- addon/calculatorClient
  This executable uses the calculatorInterface provided by the calculatorServer over ara::com
- src/calculatorServer
  This executable provides the calculatorInterface over ara::com
- vsomeipd at <communication-middleware build folder>/deamon/vsomeipd

A typical use of these three executables looks as follows:
- Start vsomeipd. It's located at <communication-middleware build folder>/deamon/vsomeipd
- Start calculatorServer so that an instance of CalculatorInterface is available.
- Start calculatorClient. It will find the instance of CalculatorInterface provided by calculatorServer and start
  using it (Method calls etc.).

4. Notes
========

PhiConv v. 4.5  04/06/2010
----------------------------

1) Description
	This program converts ASCII files from PHI - Perkin Elmer Surface Analysis Software
	for Auger Electron Spectroscopy (AES) into conventional ASCII files.
	
	Why version 3.0 and higher?
	
	This program used to be called "DifConv" (last version: 2.5.2, now discontinued). The name change reflects the ability to convert 
	not only dif files from the older DOS version of the PHI program, but also the "*.asc" files from the newer 
	MS Windows 3.1 version. For some unknown reason, PHI loves to export ASCII files which are not in 
	the conventional format (X, Y data in two tab-separated columns). This program aims at solving this 
	limitation. If you have a version of the PHI program that has been developed recently (after 1994), 
	which exhibit the same limitations, please send me the ASCII file as exported from the program, 
	and I will include it in future releases.
 
	GnuPlot (version 4.0 or higher) is required to plot ASCII files.
	GnuPlot (freeware) can be downloaded at http://www.gnuplot.info
	Curve plotting can be turned off (from the 'settings' menu) if GnuPlot is not installed. However
	some features (fitting) may not be available, since they depend on the GnuPlot package.
		
	Version 4.5 introduces several new security improvements in the code, making it stronger and less prone to fail.


2) Installation

	0. Several versions of XPS are available for your operating system. Custom builds are available for:
		1. DOS - Windows 9x (95, 98, ME): recommended for older systems.
		2. Windows 2000, XP, Vista, 7: static build, larger binary (~135KB)
		3. Windows with recent built in libraries, .NET (Windows XP SP3, Vista, 7): dynamical linked library (~100KB)

	1. Install gnuplot 4.0 or higher (available at http://gnuplot.info). Usually this means unzip the zip
	archive in a location of your choice. We strongly recommend something like:
		
		C:\Programs\gnuplot
	
	We also strongly recommend not to use C:\Program Files\gnuplot since the spaces in the folder name may
	create incompatibilities with gnuplot.
	
	2a.	Add the following line to your autoexec.bat file (located in C:/ or by running "sysedit" in the command prompt):
		SET PATH = C:\Programs\gnuplot\bin

	2b. 	Alternatively, if using Windows 2000/XP, open the "Control Panel", "System", and select "Advanced" tab. 
		Select "Environment Variables". Under the "User variables for..." press "New" and add:
		Variable name: gnuplot
		Variable value: C:\Programs\gnuplot\bin

	3. Reboot your system
 
	4. Run PhiConv.exe. You should be good to go.

	5. Alternatively drag and drop the files to be converted into the PhiConv icon. 

	Note: Two version are provided. xps.exe saves a configuration files in C:/ If you don't have 
	administration rights, xps.exe is not closing properly, so please use PhiConv_INI.exe. 	
	This version saves the configuration file in the same directory with PhiConv.cfg.
	

3) Compiling

	The source has been tested with the following compilers:
	MS VC6 and VC2008 Express, gcc 3.3, 4.0 and 4.1, 4.2, 4.3.
	The DOS-Windows9x binaries are compiled with MS VS6. The other versions are compiled with MS VS2008 Express. 
	Only standard C++ libraries are used. 

	a. Compiling using GCC 
	The best way to compile the source in this way is to install cygwin, a UNIX environment for Windows. 
	Integration with gnupolot is much tighter, so this method is actually strongly recommended. 
	Install cygwin (http://www.cygwin.com/), with the building and graphics capabilities (make, gcc, gnuplot). 		
	Use the provided makefile. To compile the binary type "make". To install it type: "make install"
	to uninstall it, type: "make uninstall". By default optimization flags (-O2) are turned on.

	b: Compiling under MS Visual Studio (MS VS6 and VS2008 Express)
	Do not use the included make file. Create a new empty project and use the
	provided source as the main source file.


4) Test file
	Test files "a.dif" (if using the DOS version of the PHI program) or "a.asc - b.asc" (if using the MS Windows version of the PHI program) are provided.


5) License

	This program (source code and binaries) is free software; 
	you can redistribute it and/or modify it under the terms of the
	GNU General Public License as published by the Free Software 
	Foundation, either in version 3 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You can find a complete copy of the GNU General Public License at:
	http://www.gnu.org/licenses/gpl.txt


6) Contact

	For any suggestion, bug, comments: Nicola Ferralis: feranick@hotmail.com
	An updated version of this program can be found at:

	http://electronsoftware.googlepages.com/phiconv


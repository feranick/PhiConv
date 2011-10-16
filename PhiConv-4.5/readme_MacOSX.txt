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
   
	Binaries for Mac OSX (PowerPC only) are provided. However, we strongly suggest to recompile phiconv from source. 
	It has been tested with XCode (based on gcc). 
	After installing gnuplot (version 4.0 or higher) and X11 for Mac OSX, xps works on Mac OSX. 

	Information on installing Mac OSX can be found online. Google for "GnuPlot OSX". An example:
	http://lee-phillips.org/info/Macintosh/gnuplot.html
	
	
3) Compiling

	The source has been tested with the following compilers:
	MS VC6, gcc 3.3, 4.0 and 4.1.
	Only standard C++ libraries are used. 

	gcc: Use the provided makefile. To compile the binary type "make". To install it type: "make install"
	to uninstall it, type: "make uninstall". By default optimization flags (-O3) are turned on.


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



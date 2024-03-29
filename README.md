# Magzter-pdf-nativeibs

Forked from mupdf library on 15-10-2012.

Latest MUPDF library is on the Branch mupdf-latest which is forked on 25-04-2016.

Modified Files:
pdf/pdf_annot.c , android/jni/mupdf.c

Features:
Rich media support

ndk-build inside the jni folder will generate the .so files and it will be used as native library for the Magzter pdf reader from this link https://github.com/magzterinc/Magzter-pdf. 

PDF reader supports the rich media content embedded inside the pdf. Also landscape mode will support the double spreaded pages.

ABOUT

MuPDF is a lightweight PDF, XPS and CBZ viewer and parser/rendering library.

The renderer in MuPDF is tailored for high quality anti-aliased graphics. It
renders text with metrics and spacing accurate to within fractions of a pixel
for the highest fidelity in reproducing the look of a printed page on screen.

MuPDF is also small, fast, and yet complete. We support PDF 1.7 with
transparency, encryption, hyperlinks, annotations, search and many other bells
and whistles. MuPDF can also read XPS documents (OpenXPS / ECMA-388),
and CBZ (Comic Book archive) files.

MuPDF is written to be both modular and portable; the example applications
are merely thin layers on top of the functionality offered by the library,
so custom viewers can be easily built for a wide range of platforms. Example
viewer applications are supplied for Windows, Linux, MacOS, iOS and Android.

MuPDF is deliberately designed to be threading library agnostic, while still
supporting multi-threaded operation. In the absence of a thread library
it will run single-threaded, but by adding one significant benefits in
rendering speed on multi-core platforms can be obtained.

We don't support interactive features such as form filling, javascript and
transitions, but such features can be added on by integrators if they so
desire.

LICENSE

MuPDF is Copyright 2006-2012 Artifex Software, Inc.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <http://www.gnu.org/licenses/>.

For commercial licensing please contact sales@artifex.com.

COMPILING

If you are compiling from source you will need several third party libraries:
freetype2, jbig2dec, libjpeg, openjpeg, and zlib. We have prepared a package
(mupdf-thirdparty.zip) that you can unzip in the mupdf source tree if you don't
have them installed on your system.

You must unpack mupdf-thirdparty.zip to build using the Visual Studio or Xcode
projects, or the example android makefiles.

DOWNLOAD

The latest development source is available directly from the git repository:

	git clone http://mupdf.com/repos/mupdf.git

INSTALLING (UNIX)

Typing "make prefix=/usr/local install" will install the binaries, man-pages,
static libraries and header files on your system.

REPORTING BUGS AND PROBLEMS

The MuPDF developers hang out on IRC in the #ghostscript channel on
irc.freenode.net.

Report bugs on the ghostscript bugzilla, with MuPDF as the selected component.

	http://bugs.ghostscript.com/

If you are reporting a problem with PDF parsing, please include the problematic
file as an attachment.


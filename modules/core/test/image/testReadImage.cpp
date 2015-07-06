/****************************************************************************
 *
 * $Id$
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2014 by INRIA. All rights reserved.
 * 
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional 
 * Edition License.
 *
 * See http://www.irisa.fr/lagadic/visp/visp.html for more information.
 * 
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * Description:
 * Read images on the disk.
 *
 * Authors:
 * Nicolas Melchior
 *
 *****************************************************************************/

#include <visp3/core/vpImage.h>
#include <visp3/core/vpImageIo.h>
#include <visp3/core/vpIoTools.h>
#include <visp3/core/vpParseArgv.h>
#include <visp3/core/vpDebug.h>

#include <stdlib.h>

/*!
  \example testReadImage.cpp

  \brief Read images on the disk.

*/

// List of allowed command line options
#define GETOPTARGS	"cdi:p:h"

void usage(const char *name, const char *badparam, std::string ipath, std::string ppath);
bool getOptions(int argc, const char **argv, std::string &ipath, std::string &ppath);

/*

  Print the program options.

  \param name : Program name.
  \param badparam : Bad parameter name.
  \param ipath: Input image path.
  \param opath : Personal image path.
  \param user : Username.

 */
void usage(const char *name, const char *badparam, std::string ipath, std::string ppath)
{
  fprintf(stdout, "\n\
Read images on the disk.\n\
\n\
SYNOPSIS\n\
  %s [-i <input image path>] [-p <personal image path>]\n\
     [-h]\n						      \
", name);

  fprintf(stdout, "\n\
OPTIONS:                                               Default\n\
  -i <input image path>                                %s\n\
     Set image input path.\n\
     From this path read \"ViSP-images/Klimt/Klimt.pgm,\n\
     .ppm, .jpeg and .png images.\n\
     Setting the VISP_INPUT_IMAGE_PATH environment\n\
     variable produces the same behaviour than using\n\
     this option.\n\
\n\
  -p <personal image path>                               %s\n\
     Set personal image path.\n\
     From this path read the image \"%s\".\n\
\n\
  -h\n\
     Print the help.\n\n",
	  ipath.c_str(), ppath.c_str(), ppath.c_str());

  if (badparam)
    fprintf(stdout, "\nERROR: Bad parameter [%s]\n", badparam);
}

/*!

  Set the program options.

  \param argc : Command line number of parameters.
  \param argv : Array of command line parameters.
  \param ipath: Input image path.
  \param ppath : Personal image path.
  \return false if the program has to be stopped, true otherwise.

*/
bool getOptions(int argc, const char **argv, std::string &ipath, std::string &ppath)
{
  const char *optarg_;
  int	c;
  while ((c = vpParseArgv::parse(argc, argv, GETOPTARGS, &optarg_)) > 1) {

    switch (c) {
    case 'i': ipath = optarg_; break;
    case 'p': ppath = optarg_; break;
    case 'h': usage(argv[0], NULL, ipath, ppath); return false; break;

    case 'c':
    case 'd':
      break;

    default:
      usage(argv[0], optarg_, ipath, ppath); return false; break;
    }
  }

  if ((c == 1) || (c == -1)) {
    // standalone param or error
    usage(argv[0], NULL, ipath, ppath);
    std::cerr << "ERROR: " << std::endl;
    std::cerr << "  Bad argument " << optarg_ << std::endl << std::endl;
    return false;
  }

  return true;
}

int
main(int argc, const char ** argv)
{
  try {
    std::string env_ipath;
    std::string opt_ipath;
    std::string opt_ppath;
    std::string ipath;
    std::string ppath;
    std::string filename;

    // Get the visp-images-data package path or VISP_INPUT_IMAGE_PATH environment variable value
    env_ipath = vpIoTools::getViSPImagesDataPath();

    // Set the default input path
    if (! env_ipath.empty())
      ipath = env_ipath;

    // Read the command line options
    if (getOptions(argc, argv, opt_ipath, opt_ppath) == false) {
      exit (-1);
    }

    // Get the option values
    if (!opt_ipath.empty())
      ipath = opt_ipath;
    if (!opt_ppath.empty())
      ppath = opt_ppath;

    // Compare ipath and env_ipath. If they differ, we take into account
    // the input path comming from the command line option
    if (!opt_ipath.empty() && !env_ipath.empty()) {
      if (ipath != env_ipath) {
        std::cout << std::endl
                  << "WARNING: " << std::endl;
        std::cout << "  Since -i <visp image path=" << ipath << "> "
                  << "  is different from VISP_IMAGE_PATH=" << env_ipath << std::endl
                  << "  we skip the environment variable." << std::endl;
      }
    }


    //
    // Here starts really the test
    //

    /////////////////////////////////////////////////////////////////////
    // Create a grey level image
    //vpImage<vpRGBa> I;
    vpImage<unsigned char> I;
    vpImage<vpRGBa> Irgb;

    if (opt_ppath.empty())
    {
      filename = vpIoTools::createFilePath(ipath, "ViSP-images/Klimt/Klimt.ppm");
      vpImageIo::read(I,filename);
      printf("Read ppm ok\n");
      filename = vpIoTools::createFilePath(ipath, "ViSP-images/Klimt/Klimt.pgm");
      vpImageIo::read(I,filename);
      printf("Read pgm ok\n");
      filename = vpIoTools::createFilePath(ipath, "ViSP-images/Klimt/Klimt.jpeg");
      vpImageIo::read(I,filename);
      printf("Read jpeg ok\n");
      filename = vpIoTools::createFilePath(ipath, "ViSP-images/Klimt/Klimt.jpg");
      vpImageIo::read(I,filename);
      printf("Read jpg ok\n");
      filename = vpIoTools::createFilePath(ipath, "ViSP-images/Klimt/Klimt.png");
      vpImageIo::read(I,filename);
      printf("Read png ok\n");

      filename = vpIoTools::createFilePath(ipath, "ViSP-images/Klimt/Klimt.ppm");
      vpImageIo::read(Irgb,filename);
      printf("Read ppm ok\n");
      filename = vpIoTools::createFilePath(ipath, "ViSP-images/Klimt/Klimt.pgm");
      vpImageIo::read(Irgb,filename);
      printf("Read pgm ok\n");
      filename = vpIoTools::createFilePath(ipath, "ViSP-images/Klimt/Klimt.jpeg");
      vpImageIo::read(Irgb,filename);
      printf("Read jpeg ok\n");
      filename = vpIoTools::createFilePath(ipath, "ViSP-images/Klimt/Klimt.jpg");
      vpImageIo::read(Irgb,filename);
      printf("Read jpg ok\n");
      filename = vpIoTools::createFilePath(ipath, "ViSP-images/Klimt/Klimt.png");
      vpImageIo::read(Irgb,filename);
      printf("Read png ok\n");
    }
    else
    {
      filename = opt_ppath;
      vpImageIo::read(I,filename);
      vpTRACE("image read without problem");
    }
  }
  catch(vpException e) {
    std::cout << "Catch an exception: " << e << std::endl;
  }
  return 0;
}
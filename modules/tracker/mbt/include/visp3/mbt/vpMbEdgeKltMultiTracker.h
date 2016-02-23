/****************************************************************************
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
 * Model-based edge klt tracker with multiple cameras.
 *
 * Authors:
 * Souriya Trinh
 *
 *****************************************************************************/

/*!
 \file vpMbEdgeKltMultiTracker.h
 \brief Model-based edge klt tracker with multiple cameras.
*/

#ifndef __vpMbEdgeKltMultiTracker_h__
#define __vpMbEdgeKltMultiTracker_h__

#include <iostream>
#include <vector>

#include <visp3/core/vpConfig.h>

#if (defined(VISP_HAVE_OPENCV) && (VISP_HAVE_OPENCV_VERSION >= 0x020100))

#include <visp3/mbt/vpMbEdgeMultiTracker.h>
#include <visp3/mbt/vpMbKltMultiTracker.h>


class VISP_EXPORT vpMbEdgeKltMultiTracker: public vpMbEdgeMultiTracker, public vpMbKltMultiTracker
{
protected:
  //! If true, compute the interaction matrix at each iteration of the minimization. Otherwise, compute it only on the first iteration.
  bool compute_interaction;
  //! The gain of the virtual visual servoing stage.
  double lambda;
  //! Factor for KLT trackers.
  double m_factorKLT;
  //! Factor for edge trackers.
  double m_factorMBT;
  //! The threshold used in the robust estimation of KLT.
  double thresholdKLT;
  //! The threshold used in the robust estimation of MBT.
  double thresholdMBT;
  //! The maximum iteration of the virtual visual servoing stage.
  unsigned int maxIter;

  //! Map of camera transformation matrix between the current camera frame to the reference camera frame (cCurrent_M_cRef)
  std::map<std::string, vpHomogeneousMatrix> m_mapOfCameraTransformationMatrix;

  //! Name of the reference camera
  std::string m_referenceCameraName;


public:
  vpMbEdgeKltMultiTracker();
  vpMbEdgeKltMultiTracker(const unsigned int nbCameras);
  vpMbEdgeKltMultiTracker(const std::vector<std::string> &cameraNames);

  virtual ~vpMbEdgeKltMultiTracker();
//
  virtual void display(const vpImage<unsigned char>& I, const vpHomogeneousMatrix &cMo_, const vpCameraParameters &cam_,
      const vpColor& col , const unsigned int thickness=1, const bool displayFullModel=false);

  virtual void display(const vpImage<vpRGBa>& I, const vpHomogeneousMatrix &cMo_, const vpCameraParameters &cam_,
      const vpColor& col , const unsigned int thickness=1, const bool displayFullModel=false);

  virtual void display(const vpImage<unsigned char>& I1, const vpImage<unsigned char>& I2, const vpHomogeneousMatrix &c1Mo,
      const vpHomogeneousMatrix &c2Mo, const vpCameraParameters &cam1, const vpCameraParameters &cam2,
      const vpColor& color, const unsigned int thickness=1, const bool displayFullModel = false);

  virtual void display(const vpImage<vpRGBa>& I1, const vpImage<vpRGBa>& I2, const vpHomogeneousMatrix &c1Mo,
      const vpHomogeneousMatrix &c2Mo, const vpCameraParameters &cam1, const vpCameraParameters &cam2,
      const vpColor& color, const unsigned int thickness=1, const bool displayFullModel = false);

  virtual void display(const std::map<std::string, const vpImage<unsigned char> *> &mapOfImages,
      const std::map<std::string, vpHomogeneousMatrix> &mapOfCameraPoses,
      const std::map<std::string, vpCameraParameters> &mapOfCameraParameters,
      const vpColor& col, const unsigned int thickness=1, const bool displayFullModel=false);

  virtual void display(const std::map<std::string, const vpImage<vpRGBa> *> &mapOfImages,
      const std::map<std::string, vpHomogeneousMatrix> &mapOfCameraPoses,
      const std::map<std::string, vpCameraParameters> &mapOfCameraParameters,
      const vpColor& col, const unsigned int thickness=1, const bool displayFullModel=false);

  virtual std::vector<std::string> getCameraNames() const;

  virtual void getCameraParameters(vpCameraParameters &camera);
  virtual void getCameraParameters(vpCameraParameters &cam1, vpCameraParameters &cam2);
  virtual void getCameraParameters(const std::string &cameraName, vpCameraParameters &cam_);
  virtual void getCameraParameters(std::map<std::string, vpCameraParameters> &mapOfCameraParameters);

  using vpMbTracker::getClipping;
  virtual unsigned int getClipping(const std::string &cameraName) const;

  virtual vpMbHiddenFaces<vpMbtPolygon>& getFaces();

  /*!
    Return the factor for the KLT trackers in the VVS process.

    \return The factor for the KLT trackers.
   */
  inline double getFactorKLT() const {
    return m_factorKLT;
  }

  /*!
    Return the factor for the MBT trackers in the VVS process.

    \return The factor for the MBT trackers.
   */
  inline double getFactorMBT() const {
    return m_factorMBT;
  }

  std::map<std::string, vpMbHiddenFaces<vpMbtPolygon> > getEdgeFaces() const;

  std::map<std::string, vpMbHiddenFaces<vpMbtPolygon> > getKltFaces() const;

  virtual std::map<std::string, std::vector<vpImagePoint> > getKltImagePoints() const;

  virtual std::map<std::string, std::map<int, vpImagePoint> > getKltImagePointsWithId() const;

  virtual std::map<std::string, vpKltOpencv> getKltOpencv() const;

#if (VISP_HAVE_OPENCV_VERSION >= 0x020408)
  virtual std::map<std::string, std::vector<cv::Point2f> > getKltPoints();
#else
  virtual std::map<std::string, CvPoint2D32f*> getKltPoints();
#endif

  virtual std::map<std::string, int> getNbKltPoints() const;

  virtual unsigned int getNbPolygon() const;
  virtual std::map<std::string, unsigned int> getEdgeMultiNbPolygon() const;
  virtual std::map<std::string, unsigned int> getKltMultiNbPolygon() const;


  /*!
    Get the number of cameras.

    \return The number of cameras.
  */
  inline unsigned int getNumberOfCameras() const {
    return (unsigned int) m_mapOfKltTrackers.size();
  }

  using vpMbTracker::getPose;
  virtual void getPose(vpHomogeneousMatrix &c1Mo, vpHomogeneousMatrix &c2Mo);
  virtual void getPose(const std::string &cameraName, vpHomogeneousMatrix &cMo_);
  virtual void getPose(std::map<std::string, vpHomogeneousMatrix> &mapOfCameraPoses);

  virtual void init(const vpImage<unsigned char>& I);

#ifdef VISP_HAVE_MODULE_GUI
  virtual void initClick(const vpImage<unsigned char>& I, const std::string& initFile, const bool displayHelp=false);

  virtual void initClick(const vpImage<unsigned char>& I1, const vpImage<unsigned char> &I2,
      const std::string& initFile1, const std::string& initFile2, const bool displayHelp=false,
      const bool firstCameraIsReference=true);

  virtual void initClick(const std::map<std::string, const vpImage<unsigned char> *> &mapOfImages,
      const std::string &initFile, const bool displayHelp=false);

  virtual void initClick(const std::map<std::string, const vpImage<unsigned char> *> &mapOfImages,
      const std::map<std::string, std::string> &mapOfInitFiles, const bool displayHelp=false);
#endif

  virtual void initCircle(const vpPoint&, const vpPoint &, const vpPoint &, const double, const int,
      const std::string &name="");
  virtual void  initCylinder(const vpPoint&, const vpPoint &, const double, const int,
      const std::string &name="");

  virtual void initFromPose(const vpImage<unsigned char>& I, const std::string &initFile);
  virtual void initFromPose(const vpImage<unsigned char>& I, const vpHomogeneousMatrix &cMo);
  virtual void initFromPose(const vpImage<unsigned char>& I, const vpPoseVector &cPo);

  virtual void initFromPose(const vpImage<unsigned char>& I1, const vpImage<unsigned char>& I2,
      const vpHomogeneousMatrix &c1Mo, const vpHomogeneousMatrix &c2Mo, const bool firstCameraIsReference=true);
  virtual void initFromPose(const std::map<std::string, const vpImage<unsigned char> *> &mapOfImages,
      const vpHomogeneousMatrix &cMo_);
  virtual void initFromPose(const std::map<std::string, const vpImage<unsigned char> *> &mapOfImages,
      const std::map<std::string, vpHomogeneousMatrix> &mapOfCameraPoses);

  virtual void initFaceFromCorners(vpMbtPolygon &polygon);
  virtual void initFaceFromLines(vpMbtPolygon &polygon);

  virtual void loadConfigFile(const std::string &configFile);

  virtual void loadConfigFile(const std::string& configFile1, const std::string& configFile2,
      const bool firstCameraIsReference=true);

  virtual void loadConfigFile(const std::map<std::string, std::string> &mapOfConfigFiles);

  virtual void loadModel(const std::string &modelFile, const bool verbose=false);

  virtual void reinit(/*const vpImage<unsigned char>& I*/);

  virtual void reInitModel(const vpImage<unsigned char>& I, const std::string &cad_name, const vpHomogeneousMatrix& cMo_,
      const bool verbose=false);
  virtual void reInitModel(const vpImage<unsigned char> &I1, const vpImage<unsigned char> &I2,
      const std::string &cad_name, const vpHomogeneousMatrix &c1Mo, const vpHomogeneousMatrix &c2Mo,
      const bool verbose=false, const bool firstCameraIsReference=true);
  virtual void reInitModel(const std::map<std::string, const vpImage<unsigned char> *> &mapOfImages,
      const std::string &cad_name, const std::map<std::string, vpHomogeneousMatrix> &mapOfCameraPoses,
      const bool verbose=false);

  virtual void resetTracker();

  virtual void setAngleAppear(const double &a);
  virtual void setAngleDisappear(const double &a);

  virtual void setCameraParameters(const vpCameraParameters& camera);

  virtual void setCameraParameters(const vpCameraParameters& camera1, const vpCameraParameters& camera2,
      const bool firstCameraIsReference=true);

  virtual void setCameraParameters(const std::string &cameraName, const vpCameraParameters& camera);

  virtual void setCameraParameters(const std::map<std::string, vpCameraParameters> &mapOfCameraParameters);

  virtual void setCameraTransformationMatrix(const std::string &cameraName,
      const vpHomogeneousMatrix &cameraTransformationMatrix);

  virtual void setCameraTransformationMatrix(const std::map<std::string, vpHomogeneousMatrix> &mapOfTransformationMatrix);

  virtual void setClipping(const unsigned int &flags);
  virtual void setClipping(const std::string &cameraName, const unsigned int &flags);

  virtual void setCovarianceComputation(const bool& flag);

  virtual void setDisplayFeatures(const bool displayF);

  virtual void setFarClippingDistance(const double &dist);
  virtual void setFarClippingDistance(const std::string &cameraName, const double &dist);

  /*!
    Set the factor for KLT tracker in the VVS process.

    \param factor : The new factor.
   */
  inline void setFactorKLT(const double factor) {
    m_factorKLT = factor;
  }

  /*!
    Set the factor for MBT tracker in the VVS process.

    \param factor : The new factor.
   */
  inline void setFactorMBT(const double factor) {
    m_factorMBT = factor;
  }

#ifdef VISP_HAVE_OGRE
  /*!
    Set the ratio of visibility attempts that has to be successful to consider a polygon as visible.

    \sa setNbRayCastingAttemptsForVisibility(const unsigned int &)

    \param ratio : Ratio of succesful attempts that has to be considered. Value has to be between 0.0 (0%) and 1.0 (100%).
  */
  void setGoodNbRayCastingAttemptsRatio(const double &ratio);

  /*!
    Set the number of rays that will be sent toward each polygon for visibility test.
    Each ray will go from the optic center of the camera to a random point inside the considered polygon.

    \sa setGoodNbRayCastingAttemptsRatio(const unsigned int &)

    \param attempts Number of rays to be sent.
  */
  void setNbRayCastingAttemptsForVisibility(const unsigned int &attempts);
#endif

  virtual void setLod(const bool useLod, const std::string &name="");
  virtual void setLod(const bool useLod, const std::string &cameraName, const std::string &name);

  virtual void setMinLineLengthThresh(const double minLineLengthThresh, const std::string &name="");
  virtual void setMinLineLengthThresh(const double minLineLengthThresh, const std::string &cameraName,
      const std::string &name);

  virtual void setMinPolygonAreaThresh(const double minPolygonAreaThresh, const std::string &name="");
  virtual void setMinPolygonAreaThresh(const double minPolygonAreaThresh, const std::string &cameraName,
      const std::string &name);

  virtual void setNearClippingDistance(const double &dist);
  virtual void setNearClippingDistance(const std::string &cameraName, const double &dist);

  virtual void setOgreShowConfigDialog(const bool showConfigDialog);

  virtual void setOgreVisibilityTest(const bool &v);

  virtual void setOptimizationMethod(const vpMbtOptimizationMethod &opt);

  virtual void setPose(const vpImage<unsigned char> &I, const vpHomogeneousMatrix &cMo);

  virtual void setPose(const vpImage<unsigned char> &I1, const vpImage<unsigned char> &I2, const vpHomogeneousMatrix &c1Mo,
      const vpHomogeneousMatrix c2Mo, const bool firstCameraIsReference=true);

  virtual void setPose(const std::map<std::string, const vpImage<unsigned char> *> &mapOfImages,
      const vpHomogeneousMatrix &cMo_);

  virtual void setPose(const std::map<std::string, const vpImage<unsigned char> *> &mapOfImages,
      const std::map<std::string, vpHomogeneousMatrix> &mapOfCameraPoses);

  virtual void setProjectionErrorComputation(const bool &flag);

  virtual void setReferenceCameraName(const std::string &referenceCameraName);

  virtual void setScanLineVisibilityTest(const bool &v);

  virtual void setThresholdAcceptation(const double th);

  virtual void setUseKltTracking(const std::string &name, const bool &useKltTracking);

  virtual void testTracking();

  virtual void track(const vpImage<unsigned char> &I);
  virtual void track(const vpImage<unsigned char>& I1, const vpImage<unsigned char>& I2);
  virtual void track(std::map<std::string, const vpImage<unsigned char> *> &mapOfImages);


protected:
  virtual void computeVVS(std::map<std::string, const vpImage<unsigned char> *> &mapOfImages,
      std::map<std::string, unsigned int> &mapOfNumberOfRows, std::map<std::string, unsigned int> &mapOfNbInfos,
      vpColVector &w_mbt, vpColVector &w_klt, const unsigned int lvl=0);

  virtual unsigned int initMbtTracking(std::vector<FeatureType> &indexOfFeatures,
      std::map<std::string, unsigned int> &mapOfNumberOfRows,
      std::map<std::string, unsigned int> &mapOfNumberOfLines,
      std::map<std::string, unsigned int> &mapOfNumberOfCylinders,
      std::map<std::string, unsigned int> &mapOfNumberOfCircles);

  virtual void postTracking(std::map<std::string, const vpImage<unsigned char> *> &mapOfImages,
      vpColVector &w_mbt, vpColVector &w_klt, std::map<std::string, unsigned int> &mapOfNumberOfRows,
      std::map<std::string, unsigned int> &mapOfNbInfos, const unsigned int lvl);

  virtual unsigned int trackFirstLoop(std::map<std::string, const vpImage<unsigned char> *> &mapOfImages, vpColVector &factor,
      std::vector<FeatureType> &indexOfFeatures,
      std::map<std::string, unsigned int> &mapOfNumberOfRows,
      std::map<std::string, unsigned int> &mapOfNumberOfLines,
      std::map<std::string, unsigned int> &mapOfNumberOfCylinders,
      std::map<std::string, unsigned int> &mapOfNumberOfCircles, const unsigned int lvl);

  virtual void trackMovingEdges(std::map<std::string, const vpImage<unsigned char> *> &mapOfImages);
};

#endif // VISP_HAVE_OPENCV
#endif //__vpMbEdgeKltMultiTracker_h__

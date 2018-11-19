// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2018.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Oliver Alka $
// $Authors: Oliver Alka $
// --------------------------------------------------------------------------

#include <OpenMS/FORMAT/DATAACCESS/FragmentAnnotation.h>
#include <OpenMS/CONCEPT/LogStream.h>
#include <fstream>
#include <QtCore/QDir>
#include <QtCore/QString>

using namespace OpenMS;
using namespace std;

namespace OpenMS
{

  void FragmentAnnotation::extractFragmentAnnotationMapping(const String& path_to_sirius_workspace, MSSpectrum& msspectrum_to_fill, bool use_exact_mass)
  {
    OpenMS::String native_id = FragmentAnnotation::extractNativeIDFromSiriusMS_(path_to_sirius_workspace);
    FragmentAnnotation::extractAnnotationFromSiriusFile_(path_to_sirius_workspace, msspectrum_to_fill, use_exact_mass);
    
    msspectrum_to_fill.setNativeID(native_id);
  }
  
  // extract native id from SIRIUS spectrum.ms output file (workspace - compound specific)
  // first native id in the spectrum.ms
  OpenMS::String FragmentAnnotation::extractNativeIDFromSiriusMS_(const String& path_to_sirius_workspace)
  {
    String ext_nid;
    const String sirius_spectrum_ms = path_to_sirius_workspace + "/spectrum.ms";
    ifstream spectrum_ms_file(sirius_spectrum_ms);
    if (spectrum_ms_file)
    {
      const OpenMS::String nid_prefix = "##nid ";
      String line;
      while (getline(spectrum_ms_file, line))
      {
        if (line.find(nid_prefix, 0) == 0)
        {
           String nid = line.erase(line.find(nid_prefix), nid_prefix.size());
           ext_nid = nid;
           break;
        }
        else if (line == ">ms1peaks")
        {
           LOG_WARN << "No native id was found - please check your input mzML. " << std::endl;
           break;
        }
      }
      spectrum_ms_file.close(); 
    }
    return ext_nid;
  }
  
  // use the first ranked sumformula (works for known and known_unkowns)
  void FragmentAnnotation::extractAnnotationFromSiriusFile_(const String& path_to_sirius_workspace, MSSpectrum& msspectrum_to_fill, bool use_exact_mass)
  { 
    if (!msspectrum_to_fill.empty()) throw Exception::IllegalArgument(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Non empty MSspectrum was provided");
    const std::string sirius_spectra_dir = path_to_sirius_workspace + "/spectra/";
    QDir dir(QString::fromStdString(sirius_spectra_dir));
    if (dir.exists())
    {
      dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
      QFileInfoList list = dir.entryInfoList();
      QFileInfo firstfile = list[0]; 
      ifstream fragment_annotation_file(firstfile.absoluteFilePath().toStdString());
      if (fragment_annotation_file)
      {
        // mz  intensity   rel.intensity   exactmass   explanation
        // 56.050855   20794.85    10.20   56.049476   C3H5N
        std::vector<Peak1D> fragments_mzs_ints;
        MSSpectrum::FloatDataArray fragments_exactmasses;
        MSSpectrum::StringDataArray fragments_explanations;
        if (use_exact_mass)
        {
          fragments_exactmasses.setName("mass");
        }
        else
        {
          fragments_exactmasses.setName("exact_mass");
        }
        fragments_explanations.setName("explanation");
        String line;
        std::getline(fragment_annotation_file, line); // skip header
        while (std::getline(fragment_annotation_file, line))
        {
          Peak1D fragment_mz_int;
          StringList splitted_line;
          line.split("\t",splitted_line);     
          // option to use the exact mass as peakMZ (e.g. for library preparation).
          if (use_exact_mass)
          {
            fragment_mz_int.setMZ(splitted_line[3].toDouble());
            fragments_exactmasses.push_back(splitted_line[0].toDouble());
          }
          else
          {
            fragment_mz_int.setMZ(splitted_line[0].toDouble());
            fragments_exactmasses.push_back(splitted_line[3].toDouble());
          }
          fragment_mz_int.setIntensity(splitted_line[1].toDouble());
          fragments_mzs_ints.push_back(fragment_mz_int);
          fragments_explanations.push_back(splitted_line[4]); 
        }
        msspectrum_to_fill.setMSLevel(2);
        msspectrum_to_fill.insert(msspectrum_to_fill.begin(), fragments_mzs_ints.begin(), fragments_mzs_ints.end());
        msspectrum_to_fill.getFloatDataArrays().push_back(fragments_exactmasses);
        msspectrum_to_fill.getStringDataArrays().push_back(fragments_explanations);
      } 
    }
    else
    {
      throw Exception::IllegalArgument(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Directory 'spectra' was not found - please check the path " + sirius_spectra_dir);
    }
  }
}

/// @endcond

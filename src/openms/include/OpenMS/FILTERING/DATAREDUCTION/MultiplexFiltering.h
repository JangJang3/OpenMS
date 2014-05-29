// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2013.
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
// $Maintainer: Lars Nilse $
// $Authors: Lars Nilse $
// --------------------------------------------------------------------------

#ifndef OPENMS_FILTERING_DATAREDUCTION_MULTIPLEXFILTERING_H
#define OPENMS_FILTERING_DATAREDUCTION_MULTIPLEXFILTERING_H

#include <OpenMS/KERNEL/StandardTypes.h>
#include <OpenMS/FILTERING/DATAREDUCTION/PeakPattern.h>
#include <OpenMS/FILTERING/DATAREDUCTION/FilterResult.h>
#include <OpenMS/MATH/MISC/Spline2d.h>
#include <OpenMS/MATH/MISC/CubicSpline2d.h>

#include <vector>
#include <algorithm>
#include <iostream>

namespace OpenMS
{
    /**
     * @brief filters peak and profile data for peak patterns
     * 
     * The algorithm searches for patterns of multiple peptides in the data.
     * The peptides appear as characteristic patterns of isotopic peaks in
     * MS1 spectra. We first search the centroided data, and optionally in
     * a second step the spline interpolated profile data. For each
     * peak pattern the algorithm generates a filter result.
     * 
     * @see PeakPattern
     * @see FilterResult
     */
    class OPENMS_DLLAPI MultiplexFiltering
    {
        
        /**
         * @brief profile and centroided experimental data
         */
        MSExperiment<Peak1D> exp_profile_;
        MSExperiment<RichPeak1D> exp_picked_;
        
        /**
         * @brief list of peak patterns
         */
        std::vector<PeakPattern> patterns_;

        /**
         * @brief maximum number of isotopic peaks per peptide
         */
        int peaks_per_peptide_max_;

        /**
         * @brief m/z shift tolerance
         */
        double mz_tolerance_;

        /**
         * @brief unit for m/z shift tolerance (ppm - true, Da - false)
         */
        bool mz_tolerance_unit_;

        public:
        /**
         * @brief constructor
         */
        MultiplexFiltering(MSExperiment<Peak1D> exp_profile, MSExperiment<RichPeak1D> exp_picked, std::vector<PeakPattern> patterns, int peaks_per_peptide_max, double mz_tolerance, bool mz_tolerance_unit);
        
        /**
         * @brief filter for patterns
         * (generates a filter result for each pattern)
         */
        std::vector<FilterResult> filter();
        
        private:
        /**
         * @brief position and blacklist filter
         */
        int positionsAndBlacklistFilter(PeakPattern pattern, std::vector<double> peak_position, int peak, std::vector<double> & mz_shifts_actual, std::vector<int> & mz_shifts_actual_indices);
        
        /**
         * @brief returns the index of a peak at m/z
         * (finds not only a valid peak, i.e. within certain m/z deviation, but the best of the valid peaks)
         */
        int getPeakIndex(std::vector<double> peak_position, int start, double mz, double scaling);
        
   };
  
}

#endif /* MULTIPLEXFILTERING_H_ */

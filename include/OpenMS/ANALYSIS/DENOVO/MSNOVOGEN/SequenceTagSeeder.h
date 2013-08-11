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
// $Maintainer: Jens Allmer $
// $Authors: Jens Allmer $
// --------------------------------------------------------------------------

#ifndef OPENMS_ANALYSIS_DENOVO_MSNOVOGEN_SEQUENCETAGSEEDER_H
#define OPENMS_ANALYSIS_DENOVO_MSNOVOGEN_SEQUENCETAGSEEDER_H

#include <OpenMS/config.h>
#include <OpenMS/ANALYSIS/DENOVO/MSNOVOGEN/Seeder.h>
#include <vector>

namespace OpenMS
{
  class OPENMS_DLLAPI SequenceTagSeeder : public Seeder
  {
public:
	class SeqTag 
	{
	  public: 
		double before_;
		double after_;
		String seq_;
	    SeqTag(const double before, const String seq, const double after) : before_(before), after_(after), seq_(seq) {}
	};

private:
	/// given a peak and an amino acid returns all peaks that are p1 + aa1 mass +/- fragment mass tolerance 
	/// distance from the given peak.
	std::vector<Peak1D> getMatchingPeaks(const MSSpectrum<>* msms, const Peak1D * p1, const Residue * aa1) const;

	/// This method extracts all sequence tags from an msms spectrum in brute force fashion.
	std::vector<OpenMS::SequenceTagSeeder::SeqTag> createSequenceTags() const;

	/// Allows to remove code duplication.
	boost::shared_ptr<Chromosome> SequenceTagSeeder::createIndividual(std::vector<OpenMS::SequenceTagSeeder::SeqTag> tags) const;

public:
	/// Default c'tor taking all necessary information
	SequenceTagSeeder(const MSSpectrum<> * spec, const double precursorMass, const double precursorMassTolerance, const double fagmentMassTolerance, const std::vector<const Residue*> aaList);

	/// Implementation of the virtual method Seeder::createIndividual.
	boost::shared_ptr<Chromosome> createIndividual() const;

	/// Overriding creation of multiple individuals since many tags are created initially and it is a waste to return just one individual.
	std::vector<boost::shared_ptr<Chromosome> > createIndividuals(const Size num) const;

  };
} // namespace

#endif // OPENMS_ANALYSIS_DENOVO_MSNOVOGEN_SEQUENCETAGSEEDER_H
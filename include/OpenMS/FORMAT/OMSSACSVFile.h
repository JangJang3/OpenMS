// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2008 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Andreas Bertsch $
// --------------------------------------------------------------------------

#ifndef OPENMS_FORMAT_OMSSACSVFILE_H
#define OPENMS_FORMAT_OMSSACSVFILE_H

#include <OpenMS/METADATA/ProteinIdentification.h>
#include <OpenMS/METADATA/PeptideIdentification.h>

#include <vector>


namespace OpenMS
{
	class String;
 	/**
 		@brief File adapter for OMSSACSV files.
 		
  	The files contain the results of the OMSSA algorithm in a comma separated manner. This file adapter is able to 
		load the data from such a file into the structures of OpenMS

  	@ingroup FileIO
  */
  class OMSSACSVFile
  {
    public:

      /// Default constructor
      OMSSACSVFile();
			/// Destructor
      virtual ~OMSSACSVFile();
      
      /**
 				@brief Loads a OMSSA file
 			
				@p filename the name of the file to read from
				@p protein_identification the protein ProteinIdentification data
				@p id_data the peptide ids of the file
      */
			void load(const String& filename, ProteinIdentification& protein_identification, std::vector<PeptideIdentification>& id_data) const throw (Exception::FileNotFound, Exception::ParseError);
  };
} // namespace OpenMS

#endif // OPENMS_FORMAT_OMSSACSVFILE_H


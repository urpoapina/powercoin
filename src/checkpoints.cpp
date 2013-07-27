// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2011-2012 Litecoin Developers
// Copyright (c) 2013 Powercoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include <string>
#include <fstream>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //

	// no checkpoint now, can be added in later releases
    static MapCheckpoints mapCheckpoints =
            boost::assign::map_list_of
            (  1, uint256("0xeaffe76c4e3ff3c814c94ce46732f887bb1bc8242b50c581445db0d6d6f76801"))
            ( 99, uint256("0xa922607fc2fe50fd30b7f043953cc045405919620e35db9274dbccb633c8a13d"))
            (127, uint256("0x94a069f3f8e4f9efa95d4e9fa700daf65b28d1cff4e62f4e2401598d2bd2ad6f"))
            (159, uint256("0x5c77fd4033fd7f8ceaa7748202d741d9b1d431a18d6f3cfbbade5878f5aa0157"))
            (226, uint256("0xc9f61ecaf05662a7cb59ca404ec8aa94404ba602605500f8d8da6fe65ea741e5"))
            (10000, uint256("0x5e7167ee9451df288acf09216e096fac571292c549005654dab98f10f00c4561"))
            (20000, uint256("0x8508182b3e3f62c97670d334ff43a2ee9a9cdc654b00007df520ca635495c2a4"))
            (30000, uint256("0x81f5d703a48fdbdd3dd17c3e6efeb058eaabf9e8e0a7d8505af578530c103c98"))
            (40000, uint256("0x5e30fab143087df0c22cf856eb74714b3b4546a904deea371d6b5d35f77acbe6"))
            (50000, uint256("0xaa2e591c66586c4ee0fcaab422ad4298674a24557fbf35517b34cfcaf52d96e4"))
            (60000, uint256("0x40e83d8b920d709d585114e68c3f67c633a4c8e855c1cc421bbf5651d1f7f72d"))
            (70000, uint256("0xadd717d8e69d9a3b83014d15323d68427aa442600457a67dd4f98f5bb61af347"))
            (80000, uint256("0x311d39e959170dec8cd5d273848415bb053b3db2fc83f3d067c06fc8e3a74cf9"))
            (90000, uint256("0xd5d111f577e091ada8159037d4a95d3ce0837e9cfe9467485a53f8f51a04c4f8"))
            (99333, uint256("0x82fbaa1592bf8bd62108c4234e150cbd5ee5d64e4d55d60b974509ff23ae7784"))
            (99520, uint256("0xb7fe05e7be88cc660bb49f906ecd20684dfb9d9880e7ad84477ca19a7d1a08d1"))
//            (99333, uint256("0x"))
			;

    bool CheckBlock(int nHeight, const uint256& hash)
    {
//        mapCheckpoints.clear();
        if(!ReadCheckpointFile()) printf("Reading checkpoint file failed\n");

//        if (fTestNet) return true; // Testnet has no checkpoints

        MapCheckpoints::const_iterator i = mapCheckpoints.find(nHeight);
        if (i == mapCheckpoints.end()) return true;
        return hash == i->second;
		// return true;
    }

    int GetTotalBlocksEstimate()
    {
        if (fTestNet) return 0;

        return mapCheckpoints.rbegin()->first;
	    // return 0;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
    //    if (fTestNet) return NULL;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, mapCheckpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
				// return NULL;
        }
        return NULL;
    }

bool ReadCheckpointFile(){

std::ifstream AcheckpointFile;
std::string AcheckpointFilename = (GetDataDir().string()+std::string("/checkpoints.dat"));
    if (!AcheckpointFile.is_open()){
      AcheckpointFile.open(AcheckpointFilename.c_str(), std::fstream::in);
      if (!AcheckpointFile) return false;
    }

    int nHeight = -1;
    uint256 hash = 0;
    while (!AcheckpointFile.eof()){
       AcheckpointFile >> nHeight;
       if (!AcheckpointFile.eof()) {
         std::string shash;
         AcheckpointFile >> shash;
         const std::vector<unsigned char> vhash(shash.begin(), shash.end());
         CBigNum bhash = CBigNum(vhash);
         hash.SetHex(shash.c_str());
         mapCheckpoints[nHeight]=hash; 
       }
    }
    AcheckpointFile.close();
    if (nHeight == -1) return false;
    return true;
}

    CBlockIndex*  CheckCheckpoints(){
        MapCheckpoints::const_reverse_iterator i = mapCheckpoints.rbegin();
        if (i == mapCheckpoints.rend()) return NULL;
        while (i->first > nBestHeight){
          if (i == mapCheckpoints.rend()) return NULL;
          i++;
        }

        CBlockIndex* pindexNew = NULL;
        CBlockIndex* proll = pindexBest;
        while (i != mapCheckpoints.rend()){
          std::map<uint256, CBlockIndex*>::iterator mi = mapBlockIndex.find(i->second);

          if (mi == mapBlockIndex.end()){
            while (proll->nHeight > i->first){
              if(!(proll = proll->pprev))
                return NULL;
            }
            pindexNew = proll->pprev;
          }
          i++;
        }
        return pindexNew;
    }
}

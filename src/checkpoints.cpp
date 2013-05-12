// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2011-2012 Litecoin Developers
// Copyright (c) 2013 Powercoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

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
			;
        /*
        boost::assign::map_list_of
        (     1, uint256("0x000000552acd41b1f9ae21fc7eb4054dce1331c8755f37f3d30a15825f695cd4"))
        (     2, uint256("0x000008559730ac71244fcca965c73077616a3cebff5e28dadeb61e8ddae631ae"))
        (  1500, uint256("0x841a2965955dd288cfa707a755d05a54e45f8bd476835ec9af4402a2b59a2967"))
        (  4032, uint256("0x9ce90e427198fc0ef05e5905ce3503725b80e26afd35a987965fd7e3d9cf0846"))
        (  8064, uint256("0xeb984353fc5190f210651f150c40b8a4bab9eeeff0b729fcb3987da694430d70"))
        ( 16128, uint256("0x602edf1859b7f9a6af809f1d9b0e6cb66fdc1d4d9dcd7a4bec03e12a1ccd153d"))
        ( 23420, uint256("0xd80fdf9ca81afd0bd2b2a90ac3a9fe547da58f2530ec874e978fce0b5101b507"))
        ( 50000, uint256("0x69dc37eb029b68f075a5012dcc0419c127672adb4f3a32882b2b3e71d07a20a6"))
        ( 80000, uint256("0x4fcb7c02f676a300503f49c764a89955a8f920b46a8cbecb4867182ecdb2e90a"))
        (120000, uint256("0xbd9d26924f05f6daa7f0155f32828ec89e8e29cee9e7121b026a7a3552ac6131"))
        (161500, uint256("0xdbe89880474f4bb4f75c227c77ba1cdc024991123b28b8418dbbf7798471ff43"))
        (179620, uint256("0x2ad9c65c990ac00426d18e446e0fd7be2ffa69e9a7dcb28358a50b2b78b9f709"))
        ;
        */

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (fTestNet) return true; // Testnet has no checkpoints

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
        if (fTestNet) return NULL;

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
}

/*
 * libretroshare/src/services p3idservice.cc
 *
 * Id interface for RetroShare.
 *
 * Copyright 2012-2012 by Robert Fernie.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */

#include "services/p3idservice.h"
#include "serialiser/rsgxsiditems.h"

#include "util/rsrandom.h"
#include <retroshare/rspeers.h>

#include <sstream>
#include <stdio.h>

/****
 * #define ID_DEBUG 1
 ****/

#define ID_REQUEST_LIST		0x0001
#define ID_REQUEST_IDENTITY	0x0002
#define ID_REQUEST_REPUTATION	0x0003
#define ID_REQUEST_OPINION	0x0004

RsIdentity *rsIdentity = NULL;

#define RSGXSID_MAX_SERVICE_STRING 1024

/********************************************************************************/
/******************* Startup / Tick    ******************************************/
/********************************************************************************/

p3IdService::p3IdService(RsGeneralDataService *gds, RsNetworkExchangeService *nes)
	: RsGxsIdExchange(gds, nes, NULL, RS_SERVICE_GXSV1_TYPE_GXSID), RsIdentity(this), 
	mIdMtx("p3IdService")
{

}

int	p3IdService::internal_tick()
{
	std::cerr << "p3IdService::internal_tick()";
	std::cerr << std::endl;

	// Disable for now.
	// background_tick();

	cache_tick();

	return 0;
}


/********************************************************************************/
/******************* RsIdentity Interface ***************************************/
/********************************************************************************/

bool p3IdService:: getNickname(const RsGxsId &id, std::string &nickname)
{
	return false;
}

bool p3IdService:: getIdDetails(const RsGxsId &id, RsIdentityDetails &details)
{
	return false;
}

bool p3IdService:: getOwnIds(std::list<RsGxsId> &ownIds)
{
	return false;
}


//
bool p3IdService::submitOpinion(uint32_t& token, RsIdOpinion &opinion)
{
	return false;
}

bool p3IdService::createIdentity(uint32_t& token, RsIdentityParameters &params)
{
	return false;
}


/********************************************************************************/
/******************* RsGixs Interface     ***************************************/
/********************************************************************************/

bool p3IdService::haveKey(const RsGxsId &id)
{
	return false;
}

bool p3IdService::havePrivateKey(const RsGxsId &id)
{
	return false;
}

bool p3IdService::requestKey(const RsGxsId &id, const std::list<PeerId> &peers)
{
	return false;
}

int  p3IdService::getKey(const RsGxsId &id, RsTlvSecurityKey &key)
{
	return -1;
}

int  p3IdService::getPrivateKey(const RsGxsId &id, RsTlvSecurityKey &key)
{
	return -1;
}


/********************************************************************************/
/******************* RsGixsReputation     ***************************************/
/********************************************************************************/

bool p3IdService::getReputation(const RsGxsId &id, const GixsReputation &rep)
{
	return false;
}


/********************************************************************************/
/******************* Get/Set Data      ******************************************/
/********************************************************************************/

bool p3IdService::getGroupData(const uint32_t &token, std::vector<RsGxsIdGroup> &groups)
{

        std::vector<RsGxsGrpItem*> grpData;
        bool ok = RsGenExchange::getGroupData(token, grpData);

        if(ok)
        {
                std::vector<RsGxsGrpItem*>::iterator vit = grpData.begin();

                for(; vit != grpData.end(); vit++)
                {
                        RsGxsIdGroupItem* item = dynamic_cast<RsGxsIdGroupItem*>(*vit);
                        RsGxsIdGroup group = item->group;
                        group.mMeta = item->meta;
                        groups.push_back(group);
                }
        }

        return ok;
}


bool p3IdService::getMsgData(const uint32_t &token, std::vector<RsGxsIdOpinion> &opinions)
{
	GxsMsgDataMap msgData;
	bool ok = RsGenExchange::getMsgData(token, msgData);
	
	if(ok)
	{
		GxsMsgDataMap::iterator mit = msgData.begin();
		
		for(; mit != msgData.end();  mit++)
		{
			RsGxsGroupId grpId = mit->first;
			std::vector<RsGxsMsgItem*>& msgItems = mit->second;
			std::vector<RsGxsMsgItem*>::iterator vit = msgItems.begin();
			
			for(; vit != msgItems.end(); vit++)
			{
				RsGxsIdOpinionItem* item = dynamic_cast<RsGxsIdOpinionItem*>(*vit);
				
				if (item)
				{
					RsGxsIdOpinion opinion = item->opinion;
					opinion.mMeta = item->meta;
					opinions.push_back(opinion);
					delete item;
				}
				else
				{
					std::cerr << "Not a IdOpinion Item, deleting!" << std::endl;
					delete *vit;
				}
			}
		}
	}
	return ok;
}

/********************************************************************************/
/********************************************************************************/
/********************************************************************************/

bool 	p3IdService::createGroup(uint32_t& token, RsGxsIdGroup &group)
{
    RsGxsIdGroupItem* item = new RsGxsIdGroupItem();
    item->group = group;
    item->meta = group.mMeta;
    RsGenExchange::publishGroup(token, item);
    return true;
}

bool 	p3IdService::createMsg(uint32_t& token, RsGxsIdOpinion &opinion)
{
    RsGxsIdOpinionItem* item = new RsGxsIdOpinionItem();
    item->opinion = opinion;
    item->meta = opinion.mMeta;
    RsGenExchange::publishMsg(token, item);
    return true;
}

/************************************************************************************/
/************************************************************************************/
/************************************************************************************/

/* Cache of recently used keys
 *
 * It is expensive to fetch the keys, so we want to keep them around if possible.
 * It only stores the immutable stuff.
 *
 * This is probably crude and crap to start with.
 * Want Least Recently Used (LRU) discard policy, without having to search whole cache.
 * Use two maps:
 *   - CacheMap[key] => data.
 *   - LRUMultiMap[AccessTS] => key
 *
 * NOTE: This could be moved to a seperate class and templated to make generic
 * as it might be generally useful.
 *
 */

bool p3IdService::cache_is_loaded(const RsGxsId &id)
{
	RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/

	std::map<RsGxsId, RsGxsIdCache>::iterator it;
	it = mCacheDataMap.find(id);
	if (it == mCacheDataMap.end())
	{
		return false;
	}
	return true;
	
}

bool p3IdService::cache_fetch(const RsGxsId &id, RsGxsIdCache &data)
{
	RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/

	std::map<RsGxsId, RsGxsIdCache>::iterator it;
	it = mCacheDataMap.find(id);
	if (it == mCacheDataMap.end())
	{
		return false;
	}
	
	data = it->second;

	/* update ts on data */
        time_t old_ts = it->second.lastUsedTs;
        time_t new_ts = time(NULL);
        it->second.lastUsedTs = new_ts;

        locked_cache_update_lrumap(id, old_ts, new_ts);

	return true;
}

bool p3IdService::cache_store(const RsGxsIdGroup &group)
{
	RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/

	// Create Cache Data.
	RsGxsIdCache cache(group);

	// For consistency
	std::map<RsGxsId, RsGxsIdCache>::iterator it;
	it = mCacheDataMap.find(cache.id);
	if (it != mCacheDataMap.end())
	{
		// ERROR.
		return false;
	}

	mCacheDataMap[cache.id] = cache;
        mCacheDataCount++;

	/* add new lrumap entry */
        time_t old_ts = 0;
        time_t new_ts = time(NULL);
        it->second.lastUsedTs = new_ts;

        locked_cache_update_lrumap(cache.id, old_ts, new_ts);

	return true;
}


bool p3IdService::locked_cache_update_lrumap(const RsGxsId &key, time_t old_ts, time_t new_ts)
{
	if (old_ts == 0)
	{
		LruData data;
		data.key = key;
		/* new insertion */
		mCacheLruMap.insert(std::make_pair(new_ts, data));
		return true;
	}

	/* find old entry */
	std::multimap<time_t, LruData>::iterator mit;
	std::multimap<time_t, LruData>::iterator sit = mCacheLruMap.lower_bound(old_ts);
	std::multimap<time_t, LruData>::iterator eit = mCacheLruMap.upper_bound(old_ts);

        for(mit = sit; mit != eit; mit++)
	{
		if (mit->second.key == key)
		{
			LruData data = mit->second;
			mCacheLruMap.erase(mit);

			if (new_ts != 0) // == 0, means remove.
			{	
				mCacheLruMap.insert(std::make_pair(new_ts, data));
			}
			return true;
		}
	}
	return false;
}


bool p3IdService::cache_resize()
{
	int count_to_clear = 0;
	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
	
		// consistency check.
		if ((mCacheDataMap.size() != mCacheDataCount) ||
			(mCacheLruMap.size() != mCacheDataCount))
		{
			// ERROR.
	
		}
	
		if (mCacheDataCount > MAX_CACHE_SIZE)
		{
			count_to_clear = mCacheDataCount - MAX_CACHE_SIZE;
		}
	}

	if (count_to_clear > 0)
	{
		cache_discard_LRU(count_to_clear);
	}
	return true;
}



bool p3IdService::cache_discard_LRU(int count_to_clear)
{
	RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/

	while(count_to_clear > 0)
	{
		std::multimap<time_t, LruData>::iterator mit = mCacheLruMap.begin();
		if (mit != mCacheLruMap.end())
		{
			LruData data = mit->second;
			mCacheLruMap.erase(mit);

			/* now clear from real cache */
			std::map<RsGxsId, RsGxsIdCache>::iterator it;
			it = mCacheDataMap.find(data.key);
			if (it == mCacheDataMap.end())
			{
				// ERROR
				std::cerr << "p3IdService::cache_discard_LRU(): ERROR Missing key: " << data.key;
				std::cerr << std::endl;
				return false;
			}
			else
			{
				mCacheDataMap.erase(it);
				mCacheDataCount--;
			}
		}
		else
		{
			// No More Data, ERROR.
			std::cerr << "p3IdService::cache_discard_LRU(): INFO more more cache data";
			std::cerr << std::endl;
			return true;
		}
		count_to_clear--;
	}
	return true;
}




/***** BELOW LOADS THE CACHE FROM GXS DATASTORE *****/

#define MIN_CYCLE_GAP	2

int	p3IdService::cache_tick()
{
	std::cerr << "p3IdService::cache_tick()";
	std::cerr << std::endl;

	// Run Background Stuff.	
	background_checkTokenRequest();

	/* every minute - run a background check */
	time_t now = time(NULL);
	bool doCycle = false;
	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
		if (now -  mCacheLoad_LastCycle > MIN_CYCLE_GAP)
		{
			doCycle = true;
			mCacheLoad_LastCycle = now;
		}
	}

	if (doCycle)
	{
		cache_start_load();
	}

	cache_check_loading();

		
	return 0;
}

bool p3IdService::cache_request_load(const RsGxsId &id)
{
	bool start = false;
	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/

		mCacheLoad_ToCache.push_back(id);

		if (time(NULL) - mCacheLoad_LastCycle > MIN_CYCLE_GAP)
		{
			start = true;
		}
	}

	if (start)
		cache_start_load();

	return true;
}


bool p3IdService::cache_start_load()
{
	/* trigger request to load missing ids into cache */
	std::list<RsGxsGroupId> groupIds;
	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/

		mCacheLoad_LastCycle = time(NULL);

		mCacheLoad_Status = 1;
		
		/* now we process the modGroupList -> a map so we can use it easily later, and create id list too */
		std::list<RsGxsId>::iterator it;
		for(it = mCacheLoad_ToCache.begin(); it != mCacheLoad_ToCache.end(); it++)
		{
			groupIds.push_back(*it); // might need conversion?
		}

		mCacheLoad_ToCache.clear();
	}

	uint32_t ansType = RS_TOKREQ_ANSTYPE_DATA; 
	RsTokReqOptionsV2 opts; 
	uint32_t token = 0;

	RsGenExchange::getTokenService()->requestGroupInfo(token, ansType, opts, groupIds);

	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
		mCacheLoad_Tokens.push_back(token);
	}
	return 1;
}

bool p3IdService::cache_check_loading()
{
	/* check the status of all active tokens */
	std::list<uint32_t> toload;
	std::list<uint32_t>::iterator it;
	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
		for(it = mCacheLoad_Tokens.begin(); it != mCacheLoad_Tokens.end();)
		{
			uint32_t token = *it;
			uint32_t status = RsGenExchange::getTokenService()->requestStatus(token);
			//checkRequestStatus(token, status, reqtype, anstype, ts);
	
			if (status == RsTokenService::GXS_REQUEST_V2_STATUS_COMPLETE)
			{
				it = mCacheLoad_Tokens.erase(it);
				toload.push_back(token);
			}
			else
			{
				it++;
			}
		}
	}

	for(it = toload.begin(); it != toload.end(); it++)
	{
		cache_load_for_token(*it);
	}
	return 1;

}

bool p3IdService::cache_load_for_token(uint32_t token)
{
	std::cerr << "p3IdService::cache_load_for_token() : " << token;
	std::cerr << std::endl;

        std::vector<RsGxsIdGroup> groups;
        std::vector<RsGxsIdGroup>::iterator vit;

        if (!getGroupData(token, groups))
	{
		std::cerr << "p3IdService::cache_load_for_token() ERROR no data";
		std::cerr << std::endl;

		return false;
	}

        for(vit = groups.begin(); vit != groups.end(); vit++)
	{
		RsGxsIdGroup &group = *vit;

		std::cerr << "p3IdService::cache_load_for_token() Loaded Id with Meta: ";
		std::cerr << group.mMeta;
		std::cerr << std::endl;

		/* cache the data */
		cache_store(group);
	}

	/* drop old entries */
	cache_resize();

	return true;
}



bool p3IdService::cache_check_consistency()
{

	return false;
}


/************************************************************************************/
/************************************************************************************/
/************************************************************************************/
/************************************************************************************/
/************************************************************************************/

std::string p3IdService::genRandomId()
{
	std::string randomId;
	for(int i = 0; i < 20; i++)
	{
		randomId += (char) ('a' + (RSRandom::random_u32() % 26));
	}
	
	return randomId;
}
	
void p3IdService::generateDummyData()
{
	RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/

	/* grab all the gpg ids... and make some ids */

	std::list<std::string> gpgids;
	std::list<std::string>::iterator it;
	
	rsPeers->getGPGAllList(gpgids);

	std::string ownId = rsPeers->getGPGOwnId();
	gpgids.push_back(ownId);

	int i;
	for(it = gpgids.begin(); it != gpgids.end(); it++)
	{
		/* create one or two for each one */
		int nIds = 1 + (RSRandom::random_u32() % 2);
		for(i = 0; i < nIds; i++)
		{
			RsGxsIdGroup id;

                	RsPeerDetails details;

			//id.mKeyId = genRandomId();
			id.mMeta.mGroupId = genRandomId();
			id.mIdType = RSID_TYPE_REALID;
			id.mGpgIdHash = genRandomId();

                	if (rsPeers->getPeerDetails(*it, details))
			{
				std::ostringstream out;
				out << details.name << "_" << i + 1;

				//id.mNickname = out.str();
				id.mMeta.mGroupName = out.str();
				
				id.mGpgIdKnown = true;
			
				id.mGpgId = *it;
				id.mGpgName = details.name;
				id.mGpgEmail = details.email;

				if (*it == ownId)
				{
					id.mIdType |= RSID_RELATION_YOURSELF;
				}
				else if (rsPeers->isGPGAccepted(*it))
				{
					id.mIdType |= RSID_RELATION_FRIEND;
				}
				else
				{
					id.mIdType |= RSID_RELATION_OTHER;
				}
				
			}
			else
			{
				std::cerr << "p3IdService::generateDummyData() missing" << std::endl;
				std::cerr << std::endl;

				id.mIdType |= RSID_RELATION_OTHER;
				//id.mNickname = genRandomId();
				id.mMeta.mGroupName = genRandomId();
				id.mGpgIdKnown = false;
			}

			//mIds[id.mKeyId] = id;
			//mIdProxy->addGroup(id);
			// STORE
			uint32_t dummyToken = 0;
			createGroup(dummyToken, id);
		}
	}

#define MAX_RANDOM_GPGIDS	10 //1000
#define MAX_RANDOM_PSEUDOIDS	50 //5000

	int nFakeGPGs = (RSRandom::random_u32() % MAX_RANDOM_GPGIDS);
	int nFakePseudoIds = (RSRandom::random_u32() % MAX_RANDOM_PSEUDOIDS);

	/* make some fake gpg ids */
	for(i = 0; i < nFakeGPGs; i++)
	{
		RsGxsIdGroup id;

                RsPeerDetails details;

		//id.mKeyId = genRandomId();
		id.mMeta.mGroupId = genRandomId();
		id.mIdType = RSID_TYPE_REALID;
		id.mGpgIdHash = genRandomId();

		id.mIdType |= RSID_RELATION_OTHER;
		//id.mNickname = genRandomId();
		id.mMeta.mGroupName = genRandomId();
		id.mGpgIdKnown = false;
		id.mGpgId = "";
		id.mGpgName = "";
		id.mGpgEmail = "";

		//mIds[id.mKeyId] = id;
		//mIdProxy->addGroup(id);
		// STORE
		uint32_t dummyToken = 0;
		createGroup(dummyToken, id);
	}

	/* make lots of pseudo ids */
	for(i = 0; i < nFakePseudoIds; i++)
	{
		RsGxsIdGroup id;

                RsPeerDetails details;

		//id.mKeyId = genRandomId();
		id.mMeta.mGroupId = genRandomId();
		id.mIdType = RSID_TYPE_PSEUDONYM;
		id.mGpgIdHash = "";

		//id.mNickname = genRandomId();
		id.mMeta.mGroupName = genRandomId();
		id.mGpgIdKnown = false;
		id.mGpgId = "";
		id.mGpgName = "";
		id.mGpgEmail = "";

		//mIds[id.mKeyId] = id;
		//mIdProxy->addGroup(id);
		// STORE
		uint32_t dummyToken = 0;
		createGroup(dummyToken, id);
	}

	//mUpdated = true;

	return;
}



std::string rsIdTypeToString(uint32_t idtype)
{
	std::string str;
	if (idtype & RSID_TYPE_REALID)
	{
		str += "GPGID ";
	}
	if (idtype & RSID_TYPE_PSEUDONYM)
	{
		str += "PSEUDO ";
	}
	if (idtype & RSID_RELATION_YOURSELF)
	{
		str += "YOURSELF ";
	}
	if (idtype & RSID_RELATION_FRIEND)
	{
		str += "FRIEND ";
	}
	if (idtype & RSID_RELATION_FOF)
	{
		str += "FOF ";
	}
	if (idtype & RSID_RELATION_OTHER)
	{
		str += "OTHER ";
	}
	if (idtype & RSID_RELATION_UNKNOWN)
	{
		str += "UNKNOWN ";
	}
	return str;
}








/************************************************************************************/
/************************************************************************************/
/************************************************************************************/
/************************************************************************************/


/* here we are running a background process that calculates the reputation scores
 * for each of the IDs....
 * 
 * As this class will be extensively used by many other threads... it is best
 * that we don't block at all. This should be in a background thread.
 * Perhaps a generic method to handle this will be advisable.... but we do that later.
 *
 * To start with we will work from the Posted service.
 *
 * 
 *
 * So Reputation....
 *   Three components:
 *     1) Your Opinion: Should override everything else.
 *     2) Implicit Factors: Know the associated GPG Key.
 *     3) Your Friends Opinions: 
 *     4) Your Friends Calculated Reputation Scores.
 *
 * Must make sure that there is no Feedback loop in the Reputation calculation.
 *
 * So: Our Score + Friends Scores => Local Reputation.
 *  Local Reputation + Friends Reputations => Final Reputation?
 *
 * Do we need to 'ignore' Non-scores?
 *   ---> This becomes like the "Best Comment" algorithm from Reddit...
 *   Use a statistical mechanism to work out a lower bound on Reputation.
 *
 * But what if your opinion is wrong?.... well likely your friends will
 * get their messages and reply... you'll see the missing message - request it - check reputation etc.
 *
 * So we are going to have three different scores (Own, Peers, (the neighbour) Hood)...
 *
 * So next question, when do we need to incrementally calculate the score?
 *  .... how often do we need to recalculate everything -> this could lead to a flux of messages. 
 *
 *
 * 
 * MORE NOTES:
 *
 *   The Opinion Messages will have to be signed by PGP or SSL Keys, to guarantee that we don't 
 * multiple votes per person... As the message system doesn't handle uniqueness in this respect, 
 * we might have to do FULL_CALC for everything - This bit TODO.
 *
 * This will make IdService quite different to the other GXS services.
 */

/************************************************************************************/
/*
 * Processing Algorithm:
 *  - Grab all Groups which have received messages. 
 *  (opt 1)-> grab latest msgs for each of these and process => score.
 *  (opt 2)-> try incremental system (people probably won't change opinions often -> just set them once)
 *      --> if not possible, fallback to full calculation.
 *
 * 
 */


#define ID_BACKGROUND_PERIOD	60

int	p3IdService::background_tick()
{
	std::cerr << "p3IdService::background_tick()";
	std::cerr << std::endl;

	// Run Background Stuff.	
	background_checkTokenRequest();

	/* every minute - run a background check */
	time_t now = time(NULL);
	bool doCheck = false;
	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
		if (now -  mLastBgCheck > ID_BACKGROUND_PERIOD)
		{
			doCheck = true;
			mLastBgCheck = now;
		}
	}

	if (doCheck)
	{
		//addExtraDummyData();
		background_requestGroups();
	}



	// Add in new votes + comments.
	return 0;
}




/***** Background Processing ****
 *
 * Process Each Message - as it arrives.
 *
 * Update 
 *
 */
#define ID_BG_IDLE						0
#define ID_BG_REQUEST_GROUPS			1
#define ID_BG_REQUEST_UNPROCESSED		2
#define ID_BG_REQUEST_FULLCALC			3

bool p3IdService::background_checkTokenRequest()
{
	uint32_t token = 0;
	uint32_t phase = 0;
	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
		if (!mBgProcessing)
		{
			return false;
		}

		token = mBgToken;
		phase = mBgPhase;
	}


	uint32_t status;
	uint32_t reqtype;
	uint32_t anstype;
	time_t ts;

        
	status = RsGenExchange::getTokenService()->requestStatus(token);
	//checkRequestStatus(token, status, reqtype, anstype, ts);
	
	if (status == RsTokenService::GXS_REQUEST_V2_STATUS_COMPLETE)
	{
		switch(phase)
		{
			case ID_BG_REQUEST_GROUPS:
				background_requestNewMessages();
				break;
			case ID_BG_REQUEST_UNPROCESSED:
				background_processNewMessages();
				break;
			case ID_BG_REQUEST_FULLCALC:
				background_processFullCalc();
				break;
			default:
				break;
		}
	}
	return true;
}


bool p3IdService::background_requestGroups()
{
	std::cerr << "p3IdService::background_requestGroups()";
	std::cerr << std::endl;

	// grab all the subscribed groups.
	uint32_t token = 0;

	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/

		if (mBgProcessing)
		{
			std::cerr << "p3IdService::background_requestGroups() ERROR Already processing, Skip this cycle";
			std::cerr << std::endl;
			return false;
		}

		mBgProcessing = true;
		mBgPhase = ID_BG_REQUEST_GROUPS;
		mBgToken = 0;
	}

	uint32_t ansType = RS_TOKREQ_ANSTYPE_SUMMARY;
	RsTokReqOptionsV2 opts; 
	std::list<std::string> groupIds;

/**
	TODO
	opts.mStatusFilter = RSGXS_GROUP_STATUS_NEWMSG;
	opts.mStatusMask = RSGXS_GROUP_STATUS_NEWMSG;
**/

	RsGenExchange::getTokenService()->requestGroupInfo(token, ansType, opts, groupIds);
	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
		mBgToken = token;
	}

	return true;
}



bool p3IdService::background_requestNewMessages()
{
	std::cerr << "p3IdService::background_requestNewMessages()";
	std::cerr << std::endl;

	std::list<RsGroupMetaData> modGroupList;
	std::list<RsGroupMetaData>::iterator it;

	std::list<std::string> groupIds;
	uint32_t token = 0;

	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
		token = mBgToken;
	}

	if (!getGroupSummary(token, modGroupList))
	{
		std::cerr << "p3IdService::background_requestNewMessages() ERROR No Group List";
		std::cerr << std::endl;
		background_cleanup();
		return false;
	}

	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
		mBgPhase = ID_BG_REQUEST_UNPROCESSED;
		mBgToken = 0;

		/* now we process the modGroupList -> a map so we can use it easily later, and create id list too */
		for(it = modGroupList.begin(); it != modGroupList.end(); it++)
		{
			/*** TODO
                        uint32_t dummyToken = 0;
			setGroupStatusFlags(dummyToken, it->mGroupId, 0, RSGXS_GROUP_STATUS_NEWMSG);
			***/

			mBgGroupMap[it->mGroupId] = *it;
			groupIds.push_back(it->mGroupId);
		}
	}

	uint32_t ansType = RS_TOKREQ_ANSTYPE_SUMMARY; 
	RsTokReqOptionsV2 opts; 
	token = 0;

	/* TODO 
	opts.mStatusFilter = RSGXS_MSG_STATUS_UNPROCESSED;
	opts.mStatusMask = RSGXS_MSG_STATUS_UNPROCESSED;
	*/

	RsGenExchange::getTokenService()->requestMsgInfo(token, ansType, opts, groupIds);

	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
		mBgToken = token;
	}
	return true;
}


bool p3IdService::background_processNewMessages()
{
	std::cerr << "p3IdService::background_processNewMessages()";
	std::cerr << std::endl;

	GxsMsgMetaMap newMsgMap;
	GxsMsgMetaMap::iterator it;
	uint32_t token = 0;

	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
		token = mBgToken;
	}

	if (!getMsgSummary(token, newMsgMap))
	{
		std::cerr << "p3IdService::background_processNewMessages() ERROR No New Msgs";
		std::cerr << std::endl;
		background_cleanup();
		return false;
	}

	/* iterate through the msgs.. update the mBgGroupMap with new data, 
	 * and flag these items as modified - so we rewrite them to the db later.
	 *
	 * If a message is not an original -> store groupId for requiring full analysis later.
         */

	std::map<std::string, RsGroupMetaData>::iterator mit;

	for (it = newMsgMap.begin(); it != newMsgMap.end(); it++)
	{
		std::vector<RsMsgMetaData>::iterator vit;
		for(vit = it->second.begin(); vit != it->second.end(); vit++)
		{
			std::cerr << "p3IdService::background_processNewMessages() new MsgId: " << vit->mMsgId;
			std::cerr << std::endl;
	
			/* flag each new vote as processed */
			/**
				TODO
			uint32_t dummyToken = 0;
			setMsgStatusFlags(dummyToken, it->mMsgId, 0, RSGXS_MSG_STATUS_UNPROCESSED);
			**/
	
			RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
	
			mit = mBgGroupMap.find(it->first);
			if (mit == mBgGroupMap.end())
			{
				std::cerr << "p3IdService::background_processNewMessages() ERROR missing GroupId: ";
				std::cerr << vit->mGroupId;
				std::cerr << std::endl;
	
				/* error */
				continue;
			}
	
			if (mit->second.mGroupStatus & ID_LOCAL_STATUS_FULL_CALC_FLAG)
			{
				std::cerr << "p3IdService::background_processNewMessages() Group Already marked FULL_CALC";
				std::cerr << std::endl;
	
				/* already marked */
				continue;
			}
	
			if (vit->mMsgId != vit->mOrigMsgId)
			{
				/*
				 *  not original -> hard, redo calc (alt: could substract previous score)
				 */
	
				std::cerr << "p3IdService::background_processNewMessages() Update, mark for FULL_CALC";
				std::cerr << std::endl;
	
				mit->second.mGroupStatus |= ID_LOCAL_STATUS_FULL_CALC_FLAG;
			}
			else
			{
				/*
				 * Try incremental calculation.
				 * - extract parameters from group.
				 * - increment, & save back.
				 * - flag group as modified.
				 */
	
				std::cerr << "p3IdService::background_processNewMessages() NewOpt, Try Inc Calc";
				std::cerr << std::endl;
	
				mit->second.mGroupStatus |= ID_LOCAL_STATUS_INC_CALC_FLAG;
	
				std::string serviceString;
				IdGroupServiceStrData ssData;
				
				if (!extractIdGroupCache(serviceString, ssData))
				{
					/* error */
					std::cerr << "p3IdService::background_processNewMessages() ERROR Extracting";
					std::cerr << std::endl;
				}
	
				/* do calcs */
				std::cerr << "p3IdService::background_processNewMessages() Extracted: ";
				std::cerr << std::endl;
	
				/* store it back in */
				std::cerr << "p3IdService::background_processNewMessages() Stored: ";
				std::cerr << std::endl;
	
				if (!encodeIdGroupCache(serviceString, ssData))
				{
					/* error */
					std::cerr << "p3IdService::background_processNewMessages() ERROR Storing";
					std::cerr << std::endl;
				}
			}
		}
	}


	/* now iterate through groups again 
	 *  -> update status as we go
	 *  -> record one requiring a full analyssis
	 */

	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/

		std::cerr << "p3IdService::background_processNewMessages() Checking Groups for Calc Type";
		std::cerr << std::endl;

		for(mit = mBgGroupMap.begin(); mit != mBgGroupMap.end(); mit++)
		{
			if (mit->second.mGroupStatus & ID_LOCAL_STATUS_FULL_CALC_FLAG)
			{
				std::cerr << "p3IdService::background_processNewMessages() FullCalc for: ";
				std::cerr << mit->second.mGroupId;
				std::cerr << std::endl;

				mBgFullCalcGroups.push_back(mit->second.mGroupId);
			}
			else if (mit->second.mGroupStatus & ID_LOCAL_STATUS_INC_CALC_FLAG)
			{
				std::cerr << "p3IdService::background_processNewMessages() IncCalc done for: ";
				std::cerr << mit->second.mGroupId;
				std::cerr << std::endl;

				/* set Cache */
                                uint32_t dummyToken = 0;
				setGroupServiceString(dummyToken, mit->second.mGroupId, mit->second.mServiceString);
			}
			else
			{
				/* why is it here? error. */
				std::cerr << "p3IdService::background_processNewMessages() ERROR for: ";
				std::cerr << mit->second.mGroupId;
				std::cerr << std::endl;
			}
		}
	}

	return background_FullCalcRequest();
}


bool p3IdService::encodeIdGroupCache(std::string &str, const IdGroupServiceStrData &data)
{
	char line[RSGXSID_MAX_SERVICE_STRING];

	snprintf(line, RSGXSID_MAX_SERVICE_STRING, "v1 {%s} {Y:%d O:%d %d %f %f R:%d %d %f %f}", 
			 data.pgpId.c_str(), data.ownScore, 
		data.opinion.count, data.opinion.nullcount, data.opinion.sum, data.opinion.sumsq,
		data.reputation.count, data.reputation.nullcount, data.reputation.sum, data.reputation.sumsq);

	str = line;
	return true;
}


bool p3IdService::extractIdGroupCache(std::string &str, IdGroupServiceStrData &data)
{
	char pgpline[RSGXSID_MAX_SERVICE_STRING];
	char scoreline[RSGXSID_MAX_SERVICE_STRING];
	
	uint32_t iOwnScore;
	IdRepCumulScore iOpin;
	IdRepCumulScore iRep;

	// split into two parts.
	if (2 != sscanf(str.c_str(), "v1 {%[^}]} {%[^}]", pgpline, scoreline))
	{
		std::cerr << "p3IdService::extractIdGroupCache() Failed to extract Two Parts";
		std::cerr << std::endl;
		return false;
	}

	std::cerr << "p3IdService::extractIdGroupCache() pgpline: " << pgpline;
	std::cerr << std::endl;
	std::cerr << "p3IdService::extractIdGroupCache() scoreline: " << scoreline;
	std::cerr << std::endl;
	
	std::string pgptmp = pgpline;
	if (pgptmp.length() > 5)
	{
		std::cerr << "p3IdService::extractIdGroupCache() Believe to have pgpId: " << pgptmp;
		std::cerr << std::endl;
		data.pgpIdKnown = true;
		data.pgpId = pgptmp;
	}
	else
	{
		std::cerr << "p3IdService::extractIdGroupCache() Think pgpId Invalid";
		std::cerr << std::endl;
		data.pgpIdKnown = false;
	}
	
	
	if (9 == sscanf(scoreline, " Y:%d O:%d %d %lf %lf R:%d %d %lf %lf", &iOwnScore, 
		&(iOpin.count), &(iOpin.nullcount), &(iOpin.sum), &(iOpin.sumsq),
		&(iRep.count), &(iRep.nullcount), &(iRep.sum), &(iRep.sumsq)))
	{
		data.ownScore = iOwnScore;
		data.opinion = iOpin;
		data.reputation = iRep;
		return true;
	}

	std::cerr << "p3IdService::extractIdGroupCache() Failed to extract scores";
	std::cerr << std::endl;

	return false;
}



bool p3IdService::background_FullCalcRequest()
{
	/* 
	 * grab an GroupId from List.
	 *  - If empty, we are finished.
	 *  - request all latest mesgs
	 */
	
	std::list<std::string> groupIds;
	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
		mBgPhase = ID_BG_REQUEST_FULLCALC;
		mBgToken = 0;
		mBgGroupMap.clear();

		if (mBgFullCalcGroups.empty())
		{
			/* finished! */
			background_cleanup();
			return true;
	
		}
	
		groupIds.push_back(mBgFullCalcGroups.front());
		mBgFullCalcGroups.pop_front();
		
	}

	/* request the summary info from the parents */
	uint32_t ansType = RS_TOKREQ_ANSTYPE_DATA; 
	uint32_t token = 0;
	RsTokReqOptionsV2 opts; 
	opts.mOptions = RS_TOKREQOPT_MSG_LATEST;

	RsGenExchange::getTokenService()->requestMsgInfo(token, ansType, opts, groupIds);

	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
		mBgToken = token;
	}
	return true;
}




bool p3IdService::background_processFullCalc()
{
	std::cerr << "p3IdService::background_processFullCalc()";
	std::cerr << std::endl;

	std::list<RsMsgMetaData> msgList;
	std::list<RsMsgMetaData>::iterator it;


	bool validmsgs = false;

	/* calc variables */
	uint32_t opinion_count = 0;
	uint32_t opinion_nullcount = 0;
	double   opinion_sum = 0;
	double   opinion_sumsq = 0;

	uint32_t rep_count = 0;
	uint32_t rep_nullcount = 0;
	double   rep_sum = 0;
	double   rep_sumsq = 0;

        std::vector<RsGxsIdOpinion> opinions;
        std::vector<RsGxsIdOpinion>::iterator vit;

        if (!getMsgData(mBgToken, opinions))
	{
		std::cerr << "p3IdService::background_processFullCalc() ERROR Failed to get Opinions";
		std::cerr << std::endl;

		return false;
	}

	std::string groupId;
        for(vit = opinions.begin(); vit != opinions.end(); vit++)
	{
		RsGxsIdOpinion &opinion = *vit;

		/* These should all be same group - should check for sanity! */
		if (groupId == "")
		{
			groupId = opinion.mMeta.mGroupId;
		}

		std::cerr << "p3IdService::background_processFullCalc() Msg:";
		std::cerr << opinion;
		std::cerr << std::endl;

		validmsgs = true;

		/* for each opinion.... extract score, and reputation */
		if (opinion.mOpinion != 0)
		{
			opinion_count++;
			opinion_sum += opinion.mOpinion;
			opinion_sum += (opinion.mOpinion * opinion.mOpinion);
		}
		else
		{
			opinion_nullcount++;
		}
		

		/* for each opinion.... extract score, and reputation */
		if (opinion.mReputation != 0)
		{
			rep_nullcount++;
			rep_sum += opinion.mReputation;
			rep_sum += (opinion.mReputation * opinion.mReputation);
		}
		else
		{
			rep_nullcount++;
		}
	}

	double opinion_avg = 0;
	double opinion_var = 0;
	double opinion_frac = 0;

	double rep_avg = 0;
	double rep_var = 0;
	double rep_frac = 0;


	if (opinion_count)
	{
		opinion_avg = opinion_sum / opinion_count;
		opinion_var = (opinion_sumsq  - opinion_count * opinion_avg * opinion_avg) / opinion_count;
		opinion_frac = opinion_count / ((float) (opinion_count + opinion_nullcount));
	}

	if (rep_count)
	{
		rep_avg = rep_sum / rep_count;
		rep_var = (rep_sumsq  - rep_count * rep_avg * rep_avg) / rep_count;
		rep_frac = rep_count / ((float) (rep_count + rep_nullcount));
	}


	if (validmsgs)
	{
		std::string serviceString;
		IdGroupServiceStrData ssData;
		
		
		if (!encodeIdGroupCache(serviceString, ssData))
		{
			std::cerr << "p3IdService::background_updateVoteCounts() Failed to encode Votes";
			std::cerr << std::endl;
		}
		else
		{
			std::cerr << "p3IdService::background_updateVoteCounts() Encoded String: " << serviceString;
			std::cerr << std::endl;
			/* store new result */
			uint32_t dummyToken = 0;
			setGroupServiceString(dummyToken, groupId, serviceString);
		}
	}

	{
		RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/
		mBgPhase = ID_BG_IDLE;
		mBgToken = 0;
	}

	return background_FullCalcRequest();
}


bool p3IdService::background_cleanup()
{
	std::cerr << "p3IdService::background_cleanup()";
	std::cerr << std::endl;

	RsStackMutex stack(mIdMtx); /********** STACK LOCKED MTX ******/

	// Cleanup.
	mBgProcessing = false;
	mBgPhase = ID_BG_IDLE;
	mBgToken = 0;
	mBgGroupMap.clear();
	mBgFullCalcGroups.clear();
	
	return true;
}


std::ostream &operator<<(std::ostream &out, const RsGxsIdGroup &grp)
{
	out << "RsGxsIdGroup: Meta: " << grp.mMeta;
	out << " IdType: " << grp.mIdType << " GpgIdHash: " << grp.mGpgIdHash;
	out << "(((Unusable: ( GpgIdKnown: " << grp.mGpgIdKnown << " GpgId: " << grp.mGpgId;
	out << " GpgName: " << grp.mGpgName << " GpgEmail: " << grp.mGpgEmail << ") )))";
	out << std::endl;
	
	return out;
}

std::ostream &operator<<(std::ostream &out, const RsGxsIdOpinion &opinion)
{
	out << "RsGxsIdOpinion: Meta: " << opinion.mMeta;
	out << std::endl;
	
	return out;
}


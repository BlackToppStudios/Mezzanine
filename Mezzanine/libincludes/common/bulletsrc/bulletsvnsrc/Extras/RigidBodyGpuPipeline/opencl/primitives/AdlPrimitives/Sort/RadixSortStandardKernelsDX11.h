static const char* radixSortStandardKernelsDX11= \
"/*\n"
"		2011 Takahiro Harada\n"
"*/\n"
"\n"
"typedef uint u32;\n"
"\n"
"#define GET_GROUP_IDX groupIdx.x\n"
"#define GET_LOCAL_IDX localIdx.x\n"
"#define GET_GLOBAL_IDX globalIdx.x\n"
"#define GROUP_LDS_BARRIER GroupMemoryBarrierWithGroupSync()\n"
"#define GROUP_MEM_FENCE\n"
"#define DEFAULT_ARGS uint3 globalIdx : SV_DispatchThreadID, uint3 localIdx : SV_GroupThreadID, uint3 groupIdx : SV_GroupID\n"
"#define AtomInc(x) InterlockedAdd(x, 1)\n"
"#define AtomInc1(x, out) InterlockedAdd(x, 1, out)\n"
"\n"
"#define make_uint4 uint4\n"
"#define make_uint2 uint2\n"
"\n"
"uint4 SELECT_UINT4(uint4 b,uint4 a,uint4 condition ){ return  make_uint4( ((condition).x)?a.x:b.x, ((condition).y)?a.y:b.y, ((condition).z)?a.z:b.z, ((condition).w)?a.w:b.w ); }\n"
"\n"
"//	takahiro end\n"
"#define WG_SIZE 128\n"
"#define NUM_PER_WI 4\n"
"\n"
"#define GET_GROUP_SIZE WG_SIZE\n"
"\n"
"typedef struct\n"
"{\n"
"	u32 m_key; \n"
"	u32 m_value;\n"
"}SortData;\n"
"\n"
"cbuffer SortCB : register( b0 )\n"
"{\n"
"	u32 m_startBit;\n"
"	u32 m_numGroups;\n"
"	u32 m_padding[2];\n"
"};\n"
"\n"
"#define BITS_PER_PASS 4\n"
"\n"
"\n"
"uint4 prefixScanVector( uint4 data )\n"
"{\n"
"	data.y += data.x;\n"
"	data.w += data.z;\n"
"	data.z += data.y;\n"
"	data.w += data.y;\n"
"	return data;\n"
"}\n"
"\n"
"uint prefixScanVectorEx( inout uint4 data )\n"
"{\n"
"	uint4 backup = data;\n"
"	data.y += data.x;\n"
"	data.w += data.z;\n"
"	data.z += data.y;\n"
"	data.w += data.y;\n"
"	uint sum = data.w;\n"
"	data -= backup;\n"
"	return sum;\n"
"}\n"
"\n"
"\n"
"\n"
"RWStructuredBuffer<SortData> sortDataIn : register( u0 );\n"
"RWStructuredBuffer<u32> ldsHistogramOut0 : register( u1 );\n"
"RWStructuredBuffer<u32> ldsHistogramOut1 : register( u2 );\n"
"\n"
"groupshared u32 ldsSortData[ WG_SIZE*NUM_PER_WI + 16 ];\n"
"\n"
"\n"
"uint4 localPrefixSum128V( uint4 pData, uint lIdx, inout uint totalSum )\n"
"{\n"
"	{	//	Set data\n"
"		ldsSortData[lIdx] = 0;\n"
"		ldsSortData[lIdx+WG_SIZE] = prefixScanVectorEx( pData );\n"
"	}\n"
"\n"
"	GROUP_LDS_BARRIER;\n"
"\n"
"	{	//	Prefix sum\n"
"		int idx = 2*lIdx + (WG_SIZE+1);\n"
"		if( lIdx < 64 )\n"
"		{\n"
"			ldsSortData[idx] += ldsSortData[idx-1];\n"
"			GROUP_MEM_FENCE;\n"
"			ldsSortData[idx] += ldsSortData[idx-2];					\n"
"			GROUP_MEM_FENCE;\n"
"			ldsSortData[idx] += ldsSortData[idx-4];\n"
"			GROUP_MEM_FENCE;\n"
"			ldsSortData[idx] += ldsSortData[idx-8];\n"
"			GROUP_MEM_FENCE;\n"
"			ldsSortData[idx] += ldsSortData[idx-16];\n"
"			GROUP_MEM_FENCE;\n"
"			ldsSortData[idx] += ldsSortData[idx-32];		\n"
"			GROUP_MEM_FENCE;\n"
"			ldsSortData[idx] += ldsSortData[idx-64];\n"
"			GROUP_MEM_FENCE;\n"
"\n"
"			ldsSortData[idx-1] += ldsSortData[idx-2];\n"
"			GROUP_MEM_FENCE;\n"
"		}\n"
"	}\n"
"\n"
"	GROUP_LDS_BARRIER;\n"
"\n"
"	totalSum = ldsSortData[WG_SIZE*2-1];\n"
"	uint addValue = ldsSortData[lIdx+127];\n"
"	return pData + make_uint4(addValue, addValue, addValue, addValue);\n"
"}\n"
"\n"
"void generateHistogram(u32 lIdx, u32 wgIdx, \n"
"		uint4 sortedData)\n"
"{\n"
"    if( lIdx < (1<<BITS_PER_PASS) )\n"
"    {\n"
"    	ldsSortData[lIdx] = 0;\n"
"    }\n"
"\n"
"	int mask = ((1<<BITS_PER_PASS)-1);\n"
"	uint4 keys = make_uint4( (sortedData.x)&mask, (sortedData.y)&mask, (sortedData.z)&mask, (sortedData.w)&mask );\n"
"\n"
"	GROUP_LDS_BARRIER;\n"
"	\n"
"	AtomInc( ldsSortData[keys.x] );\n"
"	AtomInc( ldsSortData[keys.y] );\n"
"	AtomInc( ldsSortData[keys.z] );\n"
"	AtomInc( ldsSortData[keys.w] );\n"
"}\n"
"\n"
"[numthreads(WG_SIZE, 1, 1)]\n"
"void LocalSortKernel( DEFAULT_ARGS )\n"
"{\n"
"	int nElemsPerWG = WG_SIZE*NUM_PER_WI;\n"
"	u32 lIdx = GET_LOCAL_IDX;\n"
"	u32 wgIdx = GET_GROUP_IDX;\n"
"	u32 wgSize = GET_GROUP_SIZE;\n"
"\n"
"    uint4 localAddr = make_uint4(lIdx*4+0,lIdx*4+1,lIdx*4+2,lIdx*4+3);\n"
"\n"
"\n"
"	SortData sortData[NUM_PER_WI];\n"
"\n"
"	{\n"
"		u32 offset = nElemsPerWG*wgIdx;\n"
"		sortData[0] = sortDataIn[offset+localAddr.x];\n"
"		sortData[1] = sortDataIn[offset+localAddr.y];\n"
"		sortData[2] = sortDataIn[offset+localAddr.z];\n"
"		sortData[3] = sortDataIn[offset+localAddr.w];\n"
"	}\n"
"\n"
"	int bitIdx = m_startBit;\n"
"	do\n"
"	{\n"
"//	what is this?\n"
"//		if( lIdx == wgSize-1 ) ldsSortData[256] = sortData[3].m_key;\n"
"		u32 mask = (1<<bitIdx);\n"
"		uint4 cmpResult = make_uint4( sortData[0].m_key & mask, sortData[1].m_key & mask, sortData[2].m_key & mask, sortData[3].m_key & mask );\n"
"		uint4 prefixSum = SELECT_UINT4( make_uint4(1,1,1,1), make_uint4(0,0,0,0), cmpResult != make_uint4(0,0,0,0) );\n"
"		u32 total;\n"
"		prefixSum = localPrefixSum128V( prefixSum, lIdx, total );\n"
"\n"
"		{\n"
"			uint4 dstAddr = localAddr - prefixSum + make_uint4( total, total, total, total );\n"
"			dstAddr = SELECT_UINT4( prefixSum, dstAddr, cmpResult != make_uint4(0, 0, 0, 0) );\n"
"\n"
"			GROUP_LDS_BARRIER;\n"
"\n"
"			ldsSortData[dstAddr.x] = sortData[0].m_key;\n"
"			ldsSortData[dstAddr.y] = sortData[1].m_key;\n"
"			ldsSortData[dstAddr.z] = sortData[2].m_key;\n"
"			ldsSortData[dstAddr.w] = sortData[3].m_key;\n"
"\n"
"			GROUP_LDS_BARRIER;\n"
"\n"
"			sortData[0].m_key = ldsSortData[localAddr.x];\n"
"			sortData[1].m_key = ldsSortData[localAddr.y];\n"
"			sortData[2].m_key = ldsSortData[localAddr.z];\n"
"			sortData[3].m_key = ldsSortData[localAddr.w];\n"
"\n"
"			GROUP_LDS_BARRIER;\n"
"\n"
"			ldsSortData[dstAddr.x] = sortData[0].m_value;\n"
"			ldsSortData[dstAddr.y] = sortData[1].m_value;\n"
"			ldsSortData[dstAddr.z] = sortData[2].m_value;\n"
"			ldsSortData[dstAddr.w] = sortData[3].m_value;\n"
"\n"
"			GROUP_LDS_BARRIER;\n"
"\n"
"			sortData[0].m_value = ldsSortData[localAddr.x];\n"
"			sortData[1].m_value = ldsSortData[localAddr.y];\n"
"			sortData[2].m_value = ldsSortData[localAddr.z];\n"
"			sortData[3].m_value = ldsSortData[localAddr.w];\n"
"\n"
"			GROUP_LDS_BARRIER;\n"
"		}\n"
"		bitIdx ++;\n"
"	}\n"
"	while( bitIdx <(m_startBit+BITS_PER_PASS) );\n"
"\n"
"	{	//	generate historgram\n"
"		uint4 localKeys = make_uint4( sortData[0].m_key>>m_startBit, sortData[1].m_key>>m_startBit, \n"
"			sortData[2].m_key>>m_startBit, sortData[3].m_key>>m_startBit );\n"
"\n"
"		generateHistogram( lIdx, wgIdx, localKeys );\n"
"\n"
"		GROUP_LDS_BARRIER;\n"
"\n"
"		int nBins = (1<<BITS_PER_PASS);\n"
"		if( lIdx < nBins )\n"
"		{\n"
"     		u32 histValues = ldsSortData[lIdx];\n"
"\n"
"     		u32 globalAddresses = nBins*wgIdx + lIdx;\n"
"     		u32 globalAddressesRadixMajor = m_numGroups*lIdx + wgIdx;\n"
"		\n"
"     		ldsHistogramOut0[globalAddressesRadixMajor] = histValues;\n"
"     		ldsHistogramOut1[globalAddresses] = histValues;\n"
"		}\n"
"	}\n"
"\n"
"	{	//	write\n"
"		u32 offset = nElemsPerWG*wgIdx;\n"
"		uint4 dstAddr = make_uint4(offset+localAddr.x, offset+localAddr.y, offset+localAddr.z, offset+localAddr.w );\n"
"\n"
"		sortDataIn[ dstAddr.x + 0 ] = sortData[0];\n"
"		sortDataIn[ dstAddr.x + 1 ] = sortData[1];\n"
"		sortDataIn[ dstAddr.x + 2 ] = sortData[2];\n"
"		sortDataIn[ dstAddr.x + 3 ] = sortData[3];\n"
"	}\n"
"}\n"
"\n"
"StructuredBuffer<SortData> src : register( t0 );\n"
"StructuredBuffer<u32> histogramGlobalRadixMajor : register( t1 );\n"
"StructuredBuffer<u32> histogramLocalGroupMajor : register( t2 );\n"
"\n"
"RWStructuredBuffer<SortData> dst : register( u0 );\n"
"\n"
"groupshared u32 ldsLocalHistogram[ 2*(1<<BITS_PER_PASS) ];\n"
"groupshared u32 ldsGlobalHistogram[ (1<<BITS_PER_PASS) ];\n"
"\n"
"\n"
"[numthreads(WG_SIZE, 1, 1)]\n"
"void ScatterKernel( DEFAULT_ARGS )\n"
"{\n"
"	u32 lIdx = GET_LOCAL_IDX;\n"
"	u32 wgIdx = GET_GROUP_IDX;\n"
"	u32 ldsOffset = (1<<BITS_PER_PASS);\n"
"\n"
"	//	load and prefix scan local histogram\n"
"	if( lIdx < ((1<<BITS_PER_PASS)/2) )\n"
"	{\n"
"		uint2 myIdx = make_uint2(lIdx, lIdx+8);\n"
"\n"
"		ldsLocalHistogram[ldsOffset+myIdx.x] = histogramLocalGroupMajor[(1<<BITS_PER_PASS)*wgIdx + myIdx.x];\n"
"		ldsLocalHistogram[ldsOffset+myIdx.y] = histogramLocalGroupMajor[(1<<BITS_PER_PASS)*wgIdx + myIdx.y];\n"
"		ldsLocalHistogram[ldsOffset+myIdx.x-(1<<BITS_PER_PASS)] = 0;\n"
"		ldsLocalHistogram[ldsOffset+myIdx.y-(1<<BITS_PER_PASS)] = 0;\n"
"\n"
"		int idx = ldsOffset+2*lIdx;\n"
"		ldsLocalHistogram[idx] += ldsLocalHistogram[idx-1];\n"
"		GROUP_MEM_FENCE;\n"
"		ldsLocalHistogram[idx] += ldsLocalHistogram[idx-2];\n"
"		GROUP_MEM_FENCE;\n"
"		ldsLocalHistogram[idx] += ldsLocalHistogram[idx-4];\n"
"		GROUP_MEM_FENCE;\n"
"		ldsLocalHistogram[idx] += ldsLocalHistogram[idx-8];\n"
"		GROUP_MEM_FENCE;\n"
"\n"
"		// Propagate intermediate values through\n"
"		ldsLocalHistogram[idx-1] += ldsLocalHistogram[idx-2];\n"
"		GROUP_MEM_FENCE;\n"
"\n"
"		// Grab and propagate for whole WG - loading the - 1 value\n"
"		uint2 localValues;\n"
"		localValues.x = ldsLocalHistogram[ldsOffset+myIdx.x-1];\n"
"		localValues.y = ldsLocalHistogram[ldsOffset+myIdx.y-1];\n"
"\n"
"		ldsLocalHistogram[myIdx.x] = localValues.x;\n"
"		ldsLocalHistogram[myIdx.y] = localValues.y;\n"
"\n"
"\n"
"		ldsGlobalHistogram[myIdx.x] = histogramGlobalRadixMajor[m_numGroups*myIdx.x + wgIdx];\n"
"		ldsGlobalHistogram[myIdx.y] = histogramGlobalRadixMajor[m_numGroups*myIdx.y + wgIdx];\n"
"	}\n"
"\n"
"	GROUP_LDS_BARRIER;\n"
"\n"
"    uint4 localAddr = make_uint4(lIdx*4+0,lIdx*4+1,lIdx*4+2,lIdx*4+3);\n"
"\n"
"	SortData sortData[4];\n"
"	{\n"
"	    uint4 globalAddr = wgIdx*WG_SIZE*NUM_PER_WI + localAddr;\n"
"		sortData[0] = src[globalAddr.x];\n"
"		sortData[1] = src[globalAddr.y];\n"
"		sortData[2] = src[globalAddr.z];\n"
"		sortData[3] = src[globalAddr.w];\n"
"	}\n"
"\n"
"	uint cmpValue = ((1<<BITS_PER_PASS)-1);\n"
"	uint4 radix = make_uint4( (sortData[0].m_key>>m_startBit)&cmpValue, (sortData[1].m_key>>m_startBit)&cmpValue, \n"
"		(sortData[2].m_key>>m_startBit)&cmpValue, (sortData[3].m_key>>m_startBit)&cmpValue );;\n"
"\n"
"	//	data is already sorted. So simply subtract local prefix sum\n"
"	uint4 dstAddr;\n"
"	dstAddr.x = ldsGlobalHistogram[radix.x] + (localAddr.x - ldsLocalHistogram[radix.x]);\n"
"	dstAddr.y = ldsGlobalHistogram[radix.y] + (localAddr.y - ldsLocalHistogram[radix.y]);\n"
"	dstAddr.z = ldsGlobalHistogram[radix.z] + (localAddr.z - ldsLocalHistogram[radix.z]);\n"
"	dstAddr.w = ldsGlobalHistogram[radix.w] + (localAddr.w - ldsLocalHistogram[radix.w]);\n"
"\n"
"	dst[dstAddr.x] = sortData[0];\n"
"	dst[dstAddr.y] = sortData[1];\n"
"	dst[dstAddr.z] = sortData[2];\n"
"	dst[dstAddr.w] = sortData[3];\n"
"}\n"
"\n"
"[numthreads(WG_SIZE, 1, 1)]\n"
"void CopyKernel( DEFAULT_ARGS )\n"
"{\n"
"	dst[ GET_GLOBAL_IDX ] = src[ GET_GLOBAL_IDX ];\n"
"}\n"
;

/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "KPIMonitoringModule"
 * 	found in "KPIMonitoring.asn1"
 * 	`asn1c -gen-PER`
 */

#include "CuCpResourceStatusReport.h"

static asn_TYPE_member_t asn_MBR_CuCpResourceStatusReport_1[] = {
	{ ATF_POINTER, 1, offsetof(struct CuCpResourceStatusReport, cuCpName),
		(ASN_TAG_CLASS_UNIVERSAL | (19 << 2)),
		0,
		&asn_DEF_PrintableString,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"cuCpName"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CuCpResourceStatusReport, gnbId),
		(ASN_TAG_CLASS_UNIVERSAL | (3 << 2)),
		0,
		&asn_DEF_BIT_STRING,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"gnbId"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CuCpResourceStatusReport, cuCpResourceStatus),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_CuCpResourceStatus,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"cuCpResourceStatus"
		},
};
static const int asn_MAP_CuCpResourceStatusReport_oms_1[] = { 0 };
static const ber_tlv_tag_t asn_DEF_CuCpResourceStatusReport_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_CuCpResourceStatusReport_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (3 << 2)), 1, 0, 0 }, /* gnbId */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 2, 0, 0 }, /* cuCpResourceStatus */
    { (ASN_TAG_CLASS_UNIVERSAL | (19 << 2)), 0, 0, 0 } /* cuCpName */
};
static asn_SEQUENCE_specifics_t asn_SPC_CuCpResourceStatusReport_specs_1 = {
	sizeof(struct CuCpResourceStatusReport),
	offsetof(struct CuCpResourceStatusReport, _asn_ctx),
	asn_MAP_CuCpResourceStatusReport_tag2el_1,
	3,	/* Count of tags in the map */
	asn_MAP_CuCpResourceStatusReport_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_CuCpResourceStatusReport = {
	"CuCpResourceStatusReport",
	"CuCpResourceStatusReport",
	&asn_OP_SEQUENCE,
	asn_DEF_CuCpResourceStatusReport_tags_1,
	sizeof(asn_DEF_CuCpResourceStatusReport_tags_1)
		/sizeof(asn_DEF_CuCpResourceStatusReport_tags_1[0]), /* 1 */
	asn_DEF_CuCpResourceStatusReport_tags_1,	/* Same as above */
	sizeof(asn_DEF_CuCpResourceStatusReport_tags_1)
		/sizeof(asn_DEF_CuCpResourceStatusReport_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_CuCpResourceStatusReport_1,
	3,	/* Elements count */
	&asn_SPC_CuCpResourceStatusReport_specs_1	/* Additional specs */
};

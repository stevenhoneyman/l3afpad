AM_CXXFLAGS = \
	-fno-rtti \
	-fno-exceptions \
	-fvisibility=hidden \
	-fvisibility-inlines-hidden \
	-nostdinc++ \
	-D_REENTRANT

INCLUDES = \
	-I../include \
	-I./entry \
	-I./tables

noinst_LIBRARIES = libchardetect.a

libchardetect_a_SOURCES = \
	CharDistribution.cpp \
	CharDistribution.h \
	JpCntx.cpp \
	JpCntx.h \
	LangBulgarianModel.cpp \
	LangCyrillicModel.cpp \
	LangGreekModel.cpp \
	LangHebrewModel.cpp \
	LangHungarianModel.cpp \
	LangThaiModel.cpp \
	nsBig5Prober.cpp \
	nsBig5Prober.h \
	nsCharSetProber.cpp \
	nsCharSetProber.h \
	nsCodingStateMachine.h \
	nsEUCJPProber.cpp \
	nsEUCJPProber.h \
	nsEUCKRProber.cpp \
	nsEUCKRProber.h \
	nsEUCTWProber.cpp \
	nsEUCTWProber.h \
	nsEscCharsetProber.cpp \
	nsEscCharsetProber.h \
	nsEscSM.cpp \
	nsGB2312Prober.cpp \
	nsGB2312Prober.h \
	nsHebrewProber.cpp \
	nsHebrewProber.h \
	nsLatin1Prober.cpp \
	nsLatin1Prober.h \
	nsMBCSGroupProber.cpp \
	nsMBCSGroupProber.h \
	nsMBCSSM.cpp \
	nsPkgInt.h \
	nsSBCSGroupProber.cpp \
	nsSBCSGroupProber.h \
	nsSBCharSetProber.cpp \
	nsSBCharSetProber.h \
	nsSJISProber.cpp \
	nsSJISProber.h \
	nsUTF8Prober.cpp \
	nsUTF8Prober.h \
	nsUniversalDetector.cpp \
	nsUniversalDetector.h \
	nscore.h \
	prmem.h \
	entry/impl.cpp

SUBDIRS = \
	entry \
	tables

EXTRA_DIST =

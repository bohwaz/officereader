ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=$(EPOCROOT)epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=$(EPOCROOT)epoc32\data\z
endif

TARGETDIR=$(ZDIR)\resource\apps
HEADERDIR=$(EPOCROOT)epoc32\include
ICONTARGETFILENAME=$(TARGETDIR)\mobileofficeaif.mbm
HEADERFILENAME=$(HEADERDIR)\mobileofficeaif.mbg

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE :
	mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) /c24,1 ..\aif\list_icon.bmp  /c24,1 ..\aif\context_pane_icon.bmp
	@echo mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) /c24,1 context_pane_icon.bmp context_pane_icon_mask.bmp list_icon.bmp list_icon_mask.bmp

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(HEADERFILENAME)&& \
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing



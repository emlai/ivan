# Iter Vehemens ad Necem makefile for DJGPP environment

# Copyrights (C) Timo Kiviluoto / IvanDev 2000-2003

AR       = ar rs
CC       = gxx -o
FeLibDIR = FeLib
FeLibOBJ = $(FeLibDIR)/Source/bitmap.o $(FeLibDIR)/Source/colorbit.o $(FeLibDIR)/Source/error.o $(FeLibDIR)/Source/feio.o $(FeLibDIR)/Source/felist.o $(FeLibDIR)/Source/femain.o $(FeLibDIR)/Source/femath.o $(FeLibDIR)/Source/festring.o $(FeLibDIR)/Source/graphics.o $(FeLibDIR)/Source/hscore.o $(FeLibDIR)/Source/save.o $(FeLibDIR)/Source/whandler.o
IVANDIR  = Main
IVANBIN  = Ivan.exe
IVANOBJ  = $(IVANDIR)/Source/actset.o $(IVANDIR)/Source/areaset.o $(IVANDIR)/Source/charset.o $(IVANDIR)/Source/charsset.o $(IVANDIR)/Source/config.o $(IVANDIR)/Source/coreset.o $(IVANDIR)/Source/dataset.o $(IVANDIR)/Source/dungeon.o $(IVANDIR)/Source/fluid.o $(IVANDIR)/Source/game.o $(IVANDIR)/Source/godset.o $(IVANDIR)/Source/id.o $(IVANDIR)/Source/igraph.o $(IVANDIR)/Source/itemset.o $(IVANDIR)/Source/levelset.o $(IVANDIR)/Source/main.o $(IVANDIR)/Source/materset.o $(IVANDIR)/Source/message.o $(IVANDIR)/Source/object.o $(IVANDIR)/Source/roomset.o $(IVANDIR)/Source/slotset.o $(IVANDIR)/Source/smoke.o $(IVANDIR)/Source/wmapset.o $(IVANDIR)/Source/wskill.o
FLAGS = -DGCC -IInclude -I$(FeLibDIR)/Include -O3 -ffast-math -s -W -Wall -pedantic

all:	$(IVANBIN)

$(FeLibOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS)

$(IVANOBJ) : %.o : %.cpp
	@echo Compiling $@...
	@$(CC) $@ -c $< $(FLAGS) -I$(IVANDIR)/Include

$(IVANBIN) : $(FeLibOBJ) $(IVANOBJ)
	@echo Compiling $(IVANBIN)...
	@$(CC) $(IVANBIN) $(FeLibOBJ) $(IVANOBJ) $(FLAGS)

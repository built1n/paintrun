#             __________               __   ___.
#   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
#   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
#   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
#   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
#                     \/            \/     \/    \/            \/
# $Id$
#

PAINTRUNSRCDIR := $(APPSDIR)/plugins/paintrun
PAINTRUNBUILDDIR := $(BUILDDIR)/apps/plugins/paintrun

ROCKS += $(PAINTRUNBUILDDIR)/paintrun.rock

PAINTRUN_SRC := $(call preprocess, $(PAINTRUNSRCDIR)/SOURCES)
PAINTRUN_OBJ := $(call c2obj, $(PAINTRUN_SRC))

# add source files to OTHER_SRC to get automatic dependencies
OTHER_SRC += $(PAINTRUN_SRC)

PAINTRUNFLAGS = $(filter-out -O%,$(PLUGINFLAGS)) -O2

$(PAINTRUNBUILDDIR)/paintrun.rock: $(PAINTRUN_OBJ)

$(PAINTRUNBUILDDIR)/%.o: $(PAINTRUNSRCDIR)/%.c $(PAINTRUNSRCDIR)/paintrun.make
	$(SILENT)mkdir -p $(dir $@)
	$(call PRINTS,CC $(subst $(ROOTDIR)/,,$<))$(CC) -I$(dir $<) $(PAINTRUNFLAGS) -c $< -o $@


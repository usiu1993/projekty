#!/usr/bin/env python
# -*- coding: utf-8 -*-
# crop-to-size.py: GIMP plugin to trim an image to the size specified

MINSIZE =1
MAXSIZE=12000
PROC=100
from gimpfu import *
import gtk
import os



gettext.install("gimp20-python", gimp.locale_directory, unicode=True)
wallpaperdir = os.path.join(os.getenv("HOME"), "Images/Backgrounds")

def crop_to_size(image, drw, new_width, new_height, scaling, percentage,proc,wallpaper):

	if new_width < MINSIZE or new_width > MAXSIZE or new_height<MINSIZE or new_height > MAXSIZE:
			err_msg = "Неверный размер: " + str(size)
			pdb.gimp_message_set_handler(0)
			exit(err_msg)
	
	
	gimp.pdb.gimp_undo_push_group_start(image)
	temp_image=pdb.gimp_image_duplicate(image)
	temp_drawable=pdb.gimp_image_get_active_drawable(temp_image)
	
	orginal_size=float(image.width)/float(image.height)
	new_size=float(new_width)/float(new_height)
	#SCALLING
	if scaling:
		
		if new_size > orginal_size:
			factor=float(new_height)/float(image.height)
		else:
			factor=float(new_width)/float(image.width)
		scaled_width=image.width*factor
		scaled_height=image.height*factor
		pdb.gimp_image_scale(image,scaled_width,scaled_height)	

	if percentage:
		
			if proc > PROC:
				sc_width=image.width+image.width*(proc/100)
				sc_height=image.height+image.height*(proc/100)
			else:
				sc_width=image.width*(proc/100)
				sc_height=image.height*(proc/100)
			
			pdb.gimp_image_scale(image,sc_width,sc_height)			
			#gimp.pdb.gimp_undo_push_group_end(image)
		    	
	
	if wallpaper:
			w_width = gtk.gdk.screen_width()
			w_height = gtk.gdk.screen_height()
			pdb.gimp_image_scale(image,w_width,w_height)	

			if image.name.find('.') < 0 :
				if image.filename :
					name = os.path.basename(image.filename)
				else :
            # If there's neither an image name or a filename --
            # e.g. it was created as new, or dragged/pasted from a browser --
            # make up a placeholder and hope the user notices and changes it.
					name = "wallpaper.jpg"
			else :
				name = image.name
			if name[-4:] == ".xcf" :
				name = name[0:-4] + ".jpg"
			elif name[-7:]  == ".xcf.gz" :
				name = name[0:-7] + ".jpg"
			elif name[-8:]  == ".xcf.bz2" :
				name = name[0:-8] + ".jpg"

    #print wallpaperdir, width, name
    #print img
    #print dir(img)
    #print " "
			dirpathname = os.path.join(wallpaperdir,"%dx%d" % (w_width, w_height))
			if not os.path.exists(dirpathname) :
				fulldirpathname = dirpathname
				dirpathname = os.path.join(wallpaperdir, str(w_width))
			if not os.path.exists(dirpathname) :
				errdialog = gtk.MessageDialog(None, 0,
                                          gtk.MESSAGE_ERROR, gtk.BUTTONS_OK,
                                          "Neither %s nor %s exists" %
                                           (fulldirpathname, dirpathname))
				errdialog.show_all()
				errdialog.run()
				return

	
'''
	# scaling
	if scaling:
		# calculate dimensions for scaling
		original_aspect = float(img.width) / float(img.height)
		desired_aspect = float(desired_width) / float(desired_height)
		if original_aspect < desired_aspect:
			# scale to width
			scale_factor = float(desired_width) / float(img.width)
		else:
			# scale to height
			scale_factor = float(desired_height) / float(img.height)
		scaled_width = img.width * scale_factor
		scaled_height = img.height * scale_factor
		# scale the image
		pdb.gimp_image_scale(img, scaled_width, scaled_height)
		# close undo group
		gimp.pdb.gimp_undo_push_group_end(img)

	# cropping
	# calculate starting position for cropping
	width_to_trim = img.width - desired_width
	height_to_trim = img.height - desired_height
	# calculate horizontal position for cropping
	switcher = {
		HALIGN_LEFT: lambda x: 0,
		HALIGN_CENTRE: lambda x: x/2,
		HALIGN_RIGHT: lambda x: x,
	}
	crop_x = (switcher.get(h_align))(width_to_trim)
	# calculate vertical position for cropping
	switcher = {
		VALIGN_TOP: lambda x: 0,
		VALIGN_MIDDLE: lambda x: x/2,
		VALIGN_BOTTOM: lambda x: x,
	}
	crop_y = (switcher.get(v_align))(height_to_trim)
	
	pdb.gimp_image_crop(img, desired_width, desired_height, crop_x, crop_y)
'''
		
	
register(
	"crop_to_size",
	"Trim to Size",
	_("""Trims an image to the specified dimensions, with optional scaling"""),
	"Júlio Reis",
	"© Júlio Reis – " + _("licensed via GPL v3"),
	"2016",
	_("<Image>/Image/chuj"),
	"",
	[
		(PF_INT, "number", _("Width"), 1280),
		(PF_INT, "number", _("Height"), 625),
		(PF_BOOL, "scale", _("Allow scaling"), True),
		(PF_BOOL, "percentage", _("By %"), False),
		(PF_INT, "number", _("Procent"), 125),
		(PF_BOOL, "wallpaper", _("Set Wallpaper"), False)
		#(PF_OPTION, "option", _("Horizontal alignment:"), 1, (_("Left"), _("Center"), _("Right") )),
		#(PF_OPTION, "option", _("Vertical alignment:"), 1, (_("Top"), _("Middle"), _("Bottom") ))
	],
	[],
	crop_to_size,
	domain=("gimp20-python", gimp.locale_directory)
)

main()

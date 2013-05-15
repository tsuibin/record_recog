#!/bin/bash

##################################################
# Get application's window ID
#	Input:
#		$1: application's process ID
#	Output:
#	 	$1: application's window ID
#	Return value:
#		success: 0
#		failed: 1
# Usage:
#	WID=`GetWID $PID`
#
GetWID() {
	APP=`ps --no-header -o comm -p $1`

	# Loop windows with application's executable name
	while read WID; do
		WID=`echo $WID | awk '{print $1}'`

		# Check window's PID is matching application's PID
		if [ `xprop -id $WID _NET_WM_PID | \
			awk '{print $3}'` -eq $1 ] ; then

			# Check is actual visible window
			if [ "`xwininfo -id $WID | \
				grep 'IsViewable'`" != '' ] ; then
				echo $WID
				return 0
			fi
		fi
	done < <( xwininfo -root -children 2>/dev/null | grep $APP )

	return 1
}

##################################################
# Example usage
#	1. Launch "Eye of GNOME" (EOG)
#	2. Move to top-left corner of screen
#	3. Resize to 800x600

while [ 1 ] ; do
	eog -n &
	PID=$!

	# It takes a few seconds to launch, hence retries 10 times
	for I in `seq 10` ; do
		WID=`GetWID $PID`
		if [ $? -eq 0 ] ; then
			xdotool windowmove $WID 0 0
			xdotool windowsize $WID 800 600
			exit 0
		fi
	done

	kill $PID
	zenity --question --title='Error' \
		--text='Failed to match PID and WID.' \
		--ok-label='Retry' --cancel-label='Abort program'
	if [ $? -eq 1 ] ; then
		# Abort (or Error prompt was closed)
		exit 1
	fi
done

exit 0

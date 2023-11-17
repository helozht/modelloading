#
# Regular cron jobs for the modelloading package
#
0 4	* * *	root	[ -x /usr/bin/modelloading_maintenance ] && /usr/bin/modelloading_maintenance

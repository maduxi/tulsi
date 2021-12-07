BAckup sd

https://www.anegron.site/2020/06/19/how-to-backup-your-raspberry-pi-sd-card-on-macos/

sudo dd if=/dev/rdisk2 of=name-of-my-backup.dmg # Slowest but secure
sudo dd bs=1m if=/dev/rdisk2  of=name-of-my-backup.dmg # Slightly faster and should be ok
sudo dd bs=32m if=/dev/rdisk2  of=name-of-my-backup.dmg # Very fast but riskier!

==============

Copy Image to a new SD card
Here we have to go through the same process of identifying the SD card correctly. Feel free to use either Disk Utility or the command line. For the sake of this post I will assume that the new SD card is in disk3 (it could have been disk2 again!).

We need to unmount the card by:

diskutil unmountDisk disk3
Copy
and to write the data to the new card you can choose one of the following:

sudo dd if=name-of-my-backup.dmg of=/dev/rdisk2 # Very Slow
sudo dd bs=32m if=name-of-my-backup.dmg of=/dev/rdisk2 # Ultra fast
truncate -s 8G uefi.img
sgdisk -o uefi.img
sgdisk -n 1:2048:+512M -t 1:ef00 uefi.img
sgdisk -n 2:0:0 -t 2:0700 uefi.img
sgdisk -p uefi.img
Number  Start      End        Size     Code
1       2048       ...        512M     EF00
2       ...        ...        7.5G     0700

sudo losetup -Pf uefi.img
lsblk
sudo mkfs.fat -F32 /dev/loop0p1
sudo mkfs.fat -F32 /dev/loop0p2

sudo mount -o uid=$(id -u),gid=$(id -g) /dev/loop0p1 /mnt/esp
sudo mount -o uid=$(id -u),gid=$(id -g) /dev/loop0p2 /mnt/data

# 開発ドキュメント

## イメージからUEFIを起動する

### イメージ作成
* dd if=/dev/zero of=disk.img bs=1M count=1024

### パーティションを作る
* parted disk.img
    * (parted) mklabel gpt
    * (parted) mkpart ESP fat32 1MiB 201MiB
        * これがUEFI用のブートパーティション
    * (parted) set 1 esp on
    * (parted) mkpart OS 201MiB 100%
        * これがkernelのパーティション
    * (parted) quit

### ループデバイスに関連付け
* sudo losetup -Pf disk.img

### パーティションをFAT32に
* sudo mkfs.vfat -F 32 /dev/loop0p1
* sudo mkfs.ext4 /dev/loop0p2

### ブートパーティションのマウント
* sudo mount /dev/loop0p1 /mnt
    * /mnt/EFI/BOOT/BOOTX64.EFIをここにコピーする

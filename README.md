# ROOM-STAY

## 動作環境

- デバイス: Raspberry Pi 3 ModelB+
- Raspbian OS: 4.14.X

## 環境構築

### nexmon

nexmonインストールの参考文献↓

https://github.com/seemoo-lab/nexmon

- 本リポジトリ取得

        $ git clone git@github.com:komuran/room-stay.git
        $ cd room-stay/raspberry_pi

- rootで実行されているか確認

        $ sudo su

- nexmonのリポジトリ取得

        # make nexmon/install

- 現在確認されているnexmonのバグに対応(2019/11/26時点)

        # make support/1
        # make support/2
        # make support/3
        # cd ~/nexmon/patches/bcm43455c0/7_45_154/nexmon/
        # sed -i -e 's/ifeq ($(findstring 4.9,$(shell uname -r)),4.9)/ifeq ($(findstring 4.9.,$(shell uname -r)),4.9.)/g' Makefile
        # sed -i -e 's/else ifeq ($(findstring 4.14,$(shell uname -r)),4.14)/else ifeq ($(findstring 4.14.,$(shell uname -r)),4.14.)/g' Makefile
        # reboot

- rootで実行されているか確認

        $ cd room-stay/raspberry_pi
        $ sudo su

- モード切り替えに対応する準備

        # make nexmon/setup1
        # cd ~/nexmon && source setup_env.sh && make 
        # cd /home/(ユーザ名)/room-stay/raspberry_pi
        # make nexmon/setup2

- モニターモードパッチの使用

        # iw phy `iw dev wlan0 info | gawk '/wiphy/ {printf "phy" $2}'` interface add mon0 type monitor
        # ifconfig mon0 up

- 再起動後でも自動的にドライバが使えるようにする
        
        # modinfo brcmfmac 
        (filename: /lib/modules/4.14.98-v7+/kernel/drivers/net/wireless/broadcom/$ # brcm80211/brcmfmac/brcmfmac.ko と表示された場合)
        # mv /lib/modules/4.14.98-v7+/kernel/drivers/net/wireless/broadcom/brcm80211/brcmfmac/brcmfmac.ko /lib/modules/4.14.98-v7+/kernel/drivers/net/wireless/broadcom/brcm80211/brcmfmac/brcmfmac.ko.orig
        # cp ~/nexmon/patches/bcm43455c0/7_45_154/nexmon/brcmfmac_4.14.y-nexmon/brcmfmac.ko /lib/modules/4.14.98-v7+/kernel/drivers/net/wireless/broadcom/brcm80211/brcmfmac/
        # depmod -a
        # reboot

### libtins

libtinsインストールの参考文献↓

http://libtins.github.io/download/

- rootで実行されているか確認

        $ sudo su
        $ cd

- libtinsを使えるようにする

        # apt-get update
        # apt-get upgrade
        # apt-get install libpcap-dev libssl-dev cmake
        # git clone https://github.com/mfontanini/libtins.git
        # cd ~/libtins
        # mkdir build
        # cd build
        # cmake ../
        # make
        # cmake ../ -DLIBTINS_ENABLE_CXX11=1
        # make install
        # ldconfig

### curl

curlインストールの参考文献↓

https://qiita.com/kb10uy/items/976a52f687bcb7745fc7

- rootで実行されているか確認

        $ sudo su
        $ cd

- curlを使えるようにする

        # wget http://curl.haxx.se/download/curl-7.40.0.tar.gz
        # tar xvf curl-7.40.0.tar.gz
        # cd ~/curl-7.40.0
        # ./configure --with-ssl --with-zlib
        # make
        # make install

<!---
### ruby

        $ sudo apt-get update
        $ sudo apt-get upgrade
        $ sudo apt-get install rbenv
        $ echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc
        $ echo 'eval "$(rbenv init -)"' >> ~/.bashrc
        $ git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
        $ rbenv install 2.6.2
        $ rbenv global 2.6.2

### Rails&MySQL

        $ sudo apt-get update
        $ sudo apt-get upgrade
        $ sudo apt-get install nodejs mysql-server
        $ cd room-stay
        $ gem install bundler
        $ rbenv rehash
        $ gem install rails
        $ rbenv rehash
        $ bundle install

- `$ bundle install`がうまく通らない場合`Gemfile.lock`を削除する
--->

## 本リポジトリの使い方

### capture.cppの書き換え

- 62行目の`<raspi IP>`をパケットキャプチャに使うラズパイのIPアドレスに書き換える
- 125行目の`<SSID>`をパケットキャプチャしたい対象のアクセスポイントのSSIDに書き換える

### パケットキャプチャの準備

- `wlan0`のインターフェースをdown

        $ sudo ifconfig wlan0 down

- モニターモードのインターフェースmon0を追加(無効状態)
    
        $ sudo iw phy `iw dev wlan0 info | gawk '/wiphy/ {printf "phy" $2}'` interface add mon0 type monitor

- モニターモード(Mode:Monitor)で追加されているか確認
    
        $ iwconfig mon0
        mon0    IEEE 802.11  Mode:Monitor  Frequency:5.17 GHz  Tx-Power=31 dBm
                Retry short limit:7   RTS thr:off   Fragment thr:off
                Power Management:on

- (MACで)以下のコマンドで対象のアクセスポイントのチャンネルを確認

        $ /System/Library/PrivateFrameworks/Apple80211.framework/Versions/Current/Resources/airport -s

- 確認したチャンネル数に変更(例として6ch(2.437GHz)へ変更する)
    
        $ nexutil -k6

- チャンネルが変更されているか確認
    
        $ iwconfig mon0
        mon0    IEEE 802.11  Mode:Monitor  Frequency:2.437 GHz
                Retry short limit:7   RTS thr:off   Fragment thr:off
                Power Management:on
    
- インターフェースを有効にする
    
        $ sudo ifconfig mon0 up

- capture.cppをコンパイルし，実行する

        $ cd room-stay/raspberry_pi
        $ g++ -g3 capture.cpp -o capture -ltins -lcurl
        $ sudo ./capture mon0

- 対象のアクセスポイントに通信接続があった際，パケットをとってくる

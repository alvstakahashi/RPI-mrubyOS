# RPI-mrubyOS
mrubyOS(仮称)のRaspberryPi版（beraMetal環境) 　mrubyのマルチスレッド環境　評価中です

mrubyOSのAPIを使ったmrubyプログラムのコンパイルしたバイナリーをアップロードして動作させることができます。

機能説明
1.GPIO25をオンにして起動した場合にローディングのコンソールをスキップします。
2.rs-led yamanekoさんのLED制御モジュールです。機種ごとの条件コンパイルを入れています。
3.カーネルスレッド機能　周期ハンドラ　アラームハンドラ　スレッド(ITRONタスク)機能があります。
基本的にはGR-SAKURA版と同じか、それよりも対応したmrbgemsが少ないバージョンです。

RaspberryPiの　B および　B+、RPI2で動作するはずです。　動作確認は RPI2で行っています。

ビルド環境は、gnu gcc環境でビルド可能かと思います。
自分は、Windows環境MinGW環境にてビルドしています。GCCは4.8のバイナリ版をセットアップしています。
Pleiades All in One 日本語ディストリビューション Eclipse 4.3.2 Kepler ケプラー SR2 for Windows ベースのfullバージョン　C/C++ 64bit版をいれるとセットアップされるものを使っています。

GR-SAKURAとなると持っている方が少ないかも、と思ったので、RaspberryPiを公開しました。

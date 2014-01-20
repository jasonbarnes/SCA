#!/bin/bash

#This script copies everything except the datasets into ../git
cur_dir=$(pwd)

cd ../git
rm *
rm -r alexa
rm -r auto_browser
rm -r feats
rm -r prepare

git_dir=$(pwd)

cp $cur_dir/* $git_dir/ &> /dev/null
cp -r $cur_dir/alexa $git_dir/alexa

mkdir $git_dir/auto_browser
cp $cur_dir/auto_browser/Makefile $git_dir/auto_browser/Makefile
cp $cur_dir/auto_browser/*.sh $git_dir/auto_browser
cp $cur_dir/auto_browser/*.c $git_dir/auto_browser
cp $cur_dir/auto_browser/*.h $git_dir/auto_browser
cp $cur_dir/auto_browser/*.py $git_dir/auto_browser
cp $cur_dir/auto_browser/*.txt $git_dir/auto_browser

mkdir $git_dir/prepare
cp $cur_dir/prepare/Makefile $git_dir/prepare/Makefile
cp $cur_dir/prepare/*.sh $git_dir/prepare
cp $cur_dir/prepare/*.c $git_dir/prepare
cp $cur_dir/prepare/*.h $git_dir/prepare
cp $cur_dir/prepare/*.py $git_dir/prepare
cp $cur_dir/prepare/*.txt $git_dir/prepare

mkdir $git_dir/feats
cp $cur_dir/feats/Makefile $git_dir/feats/Makefile
cp $cur_dir/feats/*.sh $git_dir/feats
cp $cur_dir/feats/*.c $git_dir/feats
cp $cur_dir/feats/*.h $git_dir/feats
cp $cur_dir/feats/*.py $git_dir/feats
cp $cur_dir/feats/*.txt $git_dir/feats

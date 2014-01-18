#!/bin/bash

for f in header*
do
	sort $f > sorted.$f
done

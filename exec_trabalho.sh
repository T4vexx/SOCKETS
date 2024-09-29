#!/bin/bash

killall worker
if [ $? -ne 0 ]
then
  echo ""
fi

killall manager
if [ $? -ne 0 ]
then
  echo ""
fi

./manager &
if [ $? -ne 0 ]
then
  echo "Erro ao iniciar o processo manager"
  exit 1
fi

for i in {0..7}
do
  ./worker $i 10 &
  if [ $? -ne 0 ] 
  then
    echo "Erro ao iniciar o processo worker $i"
    exit 1
  fi
  sleep 1
done
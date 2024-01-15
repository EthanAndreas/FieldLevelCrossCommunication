# X Com Protocol

## Table of contents

  - [Table of Contents](#table-of-contents)
  - [Abstract](#abstract)
  - [Protocol](#protocol)
  - [Encoding](#encoding)
  - [Decoding](#decoding)

## Abstract

This repository provides a cross-communication protocol. It implements the protocol itself, encoding and decoding module for network packets.

## Protocol

The protocol is defined by a structure called **XCom_Frame_t**, it is defined in *XCom_protocol.h*.

## Encoding

Encoding module takes the **XCom** protocol structure and transform it into a bytes array in **big endian**.

## Decoding

Decoding module takes a bytes array in **big endian** and transform it into **XCom** protocol structure with **little** or **big endian** in function of the current system.
(use-modules
 (srfi srfi-1)
 (guix utils)
 (guix build utils)
 (guix packages)
 (guix profiles)
 ((guix licenses)
  #:prefix license:)
 (guix build-system gnu)
 (guix build-system meson)
 (guix git-download)
 (gnu packages)
 (gnu packages autotools)
 (gnu packages cross-base)
 (gnu packages gcc))


(define binutils-arm-none-eabi
  (cross-binutils "arm-none-eabi"))
(define gcc-arm-none-eabi
  (cross-gcc "arm-none-eabi"
	     #:xgcc
	     (specification->package "gcc")
	     #:xbinutils binutils-arm-none-eabi))
(define libstdc++-arm-none-eabi
  (let
      ((libstdc++
	(make-libstdc++ gcc-arm-none-eabi)))
    (package
     (inherit libstdc++)
     (name "libstdc++-arm-none-eabi")
     (arguments
      (substitute-keyword-arguments
       (package-arguments libstdc++)
       ((#:configure-flags flags)
	`(append ,flags
		 '("--target=arm-none-eabi"
		   "--host=arm-none-eabi"
		   "--without-headers"
		   "--disable-nls"
		   "--disable-tls"
		   "--disable-shared"
		   "--disable-hosted-libstdcxx"
		   "--with-target-subdir=arm-none-eabi"
		   "LDFLAGS=-nostdlib -nostartfiles -lgcc")))))
     (native-inputs
      `(("gcc-arm-none-eabi" ,gcc-arm-none-eabi)
	("binutils-arm-none-eabi" ,binutils-arm-none-eabi))))))

(define toolchain-files
  (computed-file
   "toolchain-files"
   #~(begin
       (mkdir #$output)
       (chdir #$output)
       (call-with-output-file "arm-gcc.tc"
	 (lambda (port)
	   (format port "set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(triple arm-none-eabi)
set(gcc-bin ~a/bin)
set(binutils-bin ~a/bin)
set(libstdcxx-path ~a)
set(CMAKE_C_COMPILER ${gcc-bin}/${triple}-gcc)
set(CMAKE_CXX_COMPILER ${gcc-bin}/${triple}-g++)
set(CMAKE_ASM_COMPILER ${gcc-bin}/${triple}-gcc)
set(CMAKE_OBJCOPY ${binutils-bin}/${triple}-objcopy)

set(CMAKE_CXX_FLAGS_INIT \"-isystem ${libstdcxx-path}/include -isystem ${libstdcxx-path}/include/arm-none-eabi -ffreestanding\")
set(CMAKE_EXE_LINKER_FLAGS \"-L${libstdcxx-path}/lib -nostdlib\")
link_libraries(gcc supc++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
" #$gcc-arm-none-eabi #$binutils-arm-none-eabi #$libstdc++-arm-none-eabi))))))

(define the-manifest
  (concatenate-manifests
   (list
    (packages->manifest
     (list
      binutils-arm-none-eabi
      gcc-arm-none-eabi))
    (specifications->manifest
     (list
      "bash"
      "cmake"
      "coreutils"
      "diffutils"
      "gcc-toolchain"
      "gdb-arm-none-eabi"
      "git"
      "grep"
      "ninja"
      "nss-certs"
      "python"
      "sed"))
    (manifest
     (list (manifest-entry
	    (name "toolchain-files")
	    (version "0")
	    (item toolchain-files)))))))

the-manifest

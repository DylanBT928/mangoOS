#include <kernel.h>
#include <limine.h>
#include <stdbool.h>
#include <stddef.h>

// DO NOT CHANGE -- PLEASE CONSULT BEFORE CHANGING ------------------------------------

// Sets the base revision to version number 4 to insure compatibility between kernal
// and the bootloader. Will help to overcome shortcomings of older boot protocals and
// provides good access to 64_bit long mode and other modern niceties
// DO NOT CHANGE BASE REVISION: ALL OTHER VERSIONS ARE DEPRECATED OUR CODE WILL

__attribute__((used, section(".limine_requests"))) static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(4);

// Most Limine requests should be made volatile so the compiler does not optimize them // away. They should also be accessed at least once or marked with "used" attribute
// The framebuffer is a portion of ram that stores image data for video display, we
// need this for any kind of image to be accessed

__attribute__((used, section(".limine_requests"))) static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

// Here we set the start and end markers for the Limine requests
// these can be moved anywhere, but just dont they work right now

__attribute__((used, section(".limine_requests_start"))) static volatile uint64_t limine__requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t limine__requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

// ---------------------------------------------------------------------------------

// Halt and catch fire
// This function leads to a complete system crash, calling inline "HALT" infinite times
// This is a pretty unsafe function thats used for some debugging
// NEVER CALL THIS UNLESS YOU ABSOLUTELY NEED TO

static void hcf(void)
{
    for (;;)
    {
        asm("hlt");
    }
}

// This is our Kernel's main entry point
// If we rename this function, we have to change the linker script accordingly

void kmain(void)
{
    // Ensures the bootloader actually understands our base revision
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
    {
        // error-catching (crashing our os if it doesnt work lol)
        hcf();
    }

    // Ensures our framebuffer works
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
    {
        // error-catching
        hcf();
    }

    // We fetch the first framebuffer (only one by default) check documentation
    // for the struct layout request -> response -> framebuffer.
    struct limine_framebuffer* fb = framebuffer_request.response->framebuffers[0];

    // We assume the framebuffer model is RGB and has 32-bit pixels
    for (size_t i = 0; i < 100; ++i)
    {
        // fb_ptr points to start of frame buffer memory
        volatile uint32_t* fb_ptr = fb->address;
        // Pitch is how many bytes of VRAM you should skip to go one pixel down
        // i * pitch/4 determinds the offset on the next line and + i moves horizontal
        // This should make a white diagnal line
        fb_ptr[i * (fb->pitch / 4) + i] = 0xffffff;
    }

    hcf();
}

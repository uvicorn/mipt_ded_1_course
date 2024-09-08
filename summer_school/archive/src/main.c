#include "archive.h"
#include "utils.h"
#include <stdio.h>
#include "output.h"

#ifdef __cplusplus
#include <cstring>
#include <cstdlib>
#endif

Output readcontent(const uint8_t *filename)
{
    uint8_t *fcontent = NULL;
    int fsize = 0;
    FILE *fp;

    fp = fopen(filename, "r");
    if(fp) {
        fseek(fp, 0, SEEK_END);
        fsize = ftell(fp);
        rewind(fp);

        fcontent = (uint8_t*) malloc(sizeof(uint8_t) * fsize);
        fread(fcontent, 1, fsize, fp);

        fclose(fp);
    }
    return (Output){.buffer = fcontent, .size = fsize};
}

int main(){
    /* const uint8_t * text = """ He caught a Telegraph Avenue car that was going to Berkeley. It was crowded with youths and young men who were singing songs and ever and again barking out college yells. He studied them curiously. They were university boys. They went to the same university that she did, were in her class socially, could know her, could see her every day if they wanted to. He wondered that they did not want to, that they had been out having a good time instead of being with her that evening, talking with her, sitting around her in a worshipful and adoring circle. His thoughts wandered on. He noticed one with narrow-slitted eyes and a loose-lipped mouth. That fellow was vicious, he decided. On shipboard he would be a sneak, a whiner, a tattler. He, Martin Eden, was a better man than that fellow. The thought cheered him. It seemed to draw him nearer to Her. He began comparing himself with the students. He grew conscious of the muscled mechanism of his body and felt confident that he was physically their master. But their heads were filled with knowledge that enabled them to talk her talk,—the thought depressed him. But what was a brain for? he demanded passionately. What they had done, he could do. They had been studying about life from the books while he had been busy living life. His brain was just as full of knowledge as theirs, though it was a different kind of knowledge. How many of them could tie a lanyard knot, or take a wheel or a lookout? His life spread out before him in a series of pictures of danger and daring, hardship and toil. He remembered his failures and scrapes in the process of learning. He was that much to the good, anyway. Later on they would have to begin living life and going through the mill as he had gone. Very well. While they were busy with that, he could be learning the other side of life from the books.\nAs the car crossed the zone of scattered dwellings that separated Oakland from Berkeley, he kept a lookout for a familiar, two-story building along the front of which ran the proud sign, HIGGINBOTHAM’S CASH STORE. Martin Eden got off at this corner. He stared up for a moment at the sign. It carried a message to him beyond its mere wording. A personality of smallness and egotism and petty underhandedness seemed to emanate from the letters themselves. Bernard Higginbotham had married his sister, and he knew him well. He let himself in with a latch-key and climbed the stairs to the second floor. Here lived his brother-in-law. The grocery was below. There was a smell of stale vegetables in the air. As he groped his way across the hall he stumbled over a toy-cart, left there by one of his numerous nephews and nieces, and brought up against a door with a resounding bang. The pincher, was his thought; “too miserly to burn two cents’ worth of gas and save his boarders’ necks.\nHe fumbled for the knob and entered a lighted room, where sat his sister and Bernard Higginbotham. She was patching a pair of his trousers, while his lean body was distributed over two chairs, his feet dangling in dilapidated carpet-slippers over the edge of the second chair. He glanced across the top of the paper he was reading, showing a pair of dark, insincere, sharp-staring eyes. Martin Eden never looked at him without experiencing a sense of repulsion. What his sister had seen in the man was beyond him. The other affected him as so much vermin, and always aroused in him an impulse to crush him under his foot. Some day I’ll beat the face off of him, was the way he often consoled himself for enduring the man’s existence. The eyes, weasel-like and cruel, were looking at him complainingly. """; */
    /* Output out = encode_buffer(text, strlen(text)); */
    Output image = readcontent("123.bmp");
    Output out = encode_buffer(image.buffer, image.size);
    free(image.buffer);

    FILE* io = fopen("test_output.bmp", "wb");
    /* fprintf(io, out.buffer); */
    fwrite(out.buffer, out.size, 1, io);
    output_free(&out);
    fclose(io);
}

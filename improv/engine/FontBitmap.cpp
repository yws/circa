// Copyright (c) Andrew Fischer. See LICENSE file for license terms.

#include <ft2build.h>
#include FT_FREETYPE_H

#include "circa/circa.h"

#include "Common.h"
#include "FontBitmap.h"

FT_Library g_library = NULL;

// Font table
const int MAX_FONTS = 200;

typedef struct Font
{
    FT_Face face;
} Font;

int g_nextTableIndex = 1; // Deliberately skip font index 0
Font g_fontTable[MAX_FONTS];

FT_Face get_ft_face(FontBitmap* op)
{
    return g_fontTable[op->face].face;
}

int font_load(const char* filename, int pixelHeight)
{
    FT_Error error;

    // Load the library if necessary
    if (g_library == NULL) {
        error = FT_Init_FreeType( &g_library );
        if (error) {
            Log("Failed to initialize FreeType");
            return -1;
        }
        
        memset(g_fontTable, 0, sizeof(g_fontTable));
    }

    FT_Face face;
    error = FT_New_Face( g_library, filename, 0, &face );
    
    if ( error == FT_Err_Unknown_File_Format ) {
        Log("FreeType failed to load font file %s (unknown file format)", filename);
        return -1;
    }
    else if ( error ) {
        Log("FreeType failed to load font file %s", filename);
        return -1;
    }
    
    int index = g_nextTableIndex++;
    Font* fontEntry = &g_fontTable[index];
    fontEntry->face = face;
    
    FT_Set_Pixel_Sizes(face, 0, pixelHeight);
    
    return index;
}

int font_get_face_height(FontFace face)
{
    if (face == -1)
        return 0;
    
    FT_Face ftface = g_fontTable[face].face;
    return (ftface->ascender >> 6) + ((-ftface->descender) >> 6);
}

void load_glyph(FontBitmap* op, int charIndex)
{
    FT_UInt glyph_index;
    
    FT_Face face = get_ft_face(op);
    
    glyph_index = FT_Get_Char_Index( face, op->str[charIndex] );
    
    // Load glyph image into the slot (erase previous one)
    FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
}

void font_update_metrics(FontBitmap* op)
{
    FT_Face face = get_ft_face(op);

    int overallWidth = 0;
    int highestBearingY = 0;
    int penX = 0;

    for (int i = 0 ; op->str[i] != 0; i++ ) {
        load_glyph(op, i);

        int left = penX + (face->glyph->metrics.horiBearingX >> 6);
        int right = left + (face->glyph->metrics.width >> 6);
        penX += face->glyph->advance.x >> 6;
        
        if (i == 0)
            op->originX = left;
        
        int bearingY = face->glyph->metrics.horiBearingY >> 6;
        if (bearingY > highestBearingY)
            highestBearingY = bearingY;

        overallWidth = right;
    }

    op->textWidth = overallWidth;
    
    // metrics.ascender seems to be way too high
    //op->ascent = face->size->metrics.ascender >> 6;
    op->ascent = highestBearingY;
    op->descent = face->size->metrics.descender >> 6;
    op->originY = highestBearingY;
    
    op->bitmapSizeX = overallWidth;
    op->bitmapSizeY = face->size->metrics.height >> 6;//(face->bbox.yMax - face->bbox.yMin) >> 6;
}

bool font_render(FontBitmap* op)
{
    if (op->face == -1)
        return false;
    if (op->str == NULL)
        return false;
    
    FT_Face face = get_ft_face(op);
    FT_Error error;

    int bitmapSize = op->bitmapSizeX * op->bitmapSizeY;
    op->bitmap = (char*) malloc(bitmapSize);
    
    memset(op->bitmap, 0, bitmapSize);

    // Pen will be used as the "origin" position.
    FT_Vector pen;
    pen.x = 0;
    pen.y = op->originY;
    
    for (int i = 0 ; op->str[i] != 0; i++ ) {

        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index( face, op->str[i] );
        
        // Load glyph image into the slot (erase previous one)
        error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
        if ( error )
            continue;
        
        // Convert to an anti-aliased bitmap
        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
        if ( error )
            continue;

        // Copy bitmap to target
        FT_GlyphSlot slot = face->glyph;
        FT_Bitmap* bitmap = &slot->bitmap;
        
        float horiBearingX = (slot->metrics.horiBearingX >> 6);
        float horiBearingY = (slot->metrics.horiBearingY >> 6);
        
        for (int ySource = 0; ySource < bitmap->rows; ySource++) {
            for (int xSource = 0; xSource < bitmap->width; xSource++) {
                unsigned char pixel = bitmap->buffer[ySource * bitmap->width + xSource];
                
                int xDest = pen.x + xSource + horiBearingX;
                int yDest = pen.y + ySource - horiBearingY;
                
                if (xDest < 0 || yDest < 0
                        || xDest >= op->bitmapSizeX
                        || yDest >= op->bitmapSizeY)
                    continue;
                
                op->bitmap[yDest*op->bitmapSizeX + xDest] = pixel;
            }
        }
        
        // Advance pen position
        pen.x += face->glyph->advance.x >> 6;
        pen.y += face->glyph->advance.y >> 6;
    }

    return true;
}

void font_cleanup_operation(FontBitmap* op)
{
    free(op->bitmap);
    op->bitmap = NULL;
}

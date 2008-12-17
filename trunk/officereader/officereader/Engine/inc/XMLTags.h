/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008,2009,2010 by ODENDAHL SEPT-Solutions
 *
 * Office Reader - a OpenDocument viewer for Symbian OS
 *
 * $Id:$
 *
 * This file is part of Office Reader
 *
 * Office Reader is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 or GNU 
 * General Lesser Public License version 3 , as published by the Free Software 
 * Foundation.
 *
 * Office Reader is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General (Lesser) Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU (Lesser) General Public License
 * version 3 along with Office Reader.  If not, 
 * see <http://www.gnu.org/licenses/>.
 *
 ************************************************************************/

#ifndef __XMLTags_H__
#define __XMLTags_H__

_LIT8(writer,"application/vnd.oasis.opendocument.text");
_LIT8(spread,"application/vnd.oasis.opendocument.spreadsheet");
_LIT8(presentation,"application/vnd.oasis.opendocument.presentation");
_LIT8(text_template,"application/vnd.oasis.opendocument.text-template");
_LIT8(spread_template,"application/vnd.oasis.opendocument.spreadsheet-template");
_LIT8(presentation_template,"application/vnd.oasis.opendocument.presentation-template");

_LIT(xml_encoding,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
_LIT(mimetype,"mimetype");
_LIT(styles,"styles.xml");
_LIT(content,"content.xml");
_LIT(manifest_xml,"META-INF\\manifest.xml");



//style
_LIT(style,"style");
_LIT(default_style,"default-style");
_LIT(master_page,"master-page");
_LIT(page_layout_name,"page-layout-name");

_LIT(name,"name");
_LIT(family,"family");
_LIT(text,"text");
_LIT(paragraph,"paragraph");
_LIT(style_name,"style-name");
_LIT(parent_style_name,"parent-style-name");

_LIT(list_style_name,"list-style-name");

//text-properties
_LIT(text_properties,"text-properties");
_LIT(color,"color");
_LIT(text_position,"text-position");
_LIT(font_size,"font-size");
_LIT(background_color,"background-color");
_LIT(font_weight,"font-weight");
_LIT(font_style,"font-style");
_LIT(text_underline_style,"text-underline-style");
_LIT(text_underline_type,"text-underline-type");
_LIT(text_line_through_style,"text-line-through-style");

_LIT(transparent,"transparent");


//paragraph-properties
_LIT(paragraph_properties,"paragraph-properties");
_LIT(text_align,"text-align");
_LIT(text_indent,"text-indent");
_LIT(margin_left,"margin-left");
_LIT(margin_right,"margin-right");
_LIT(border,"border");
_LIT(padding,"padding");

//graphic-properties
_LIT(graphic_properties,"graphic-properties");
_LIT(horizontal_pos,"horizontal-pos");

// table
_LIT(table,"table");
_LIT(table_row,"table-row");
_LIT(table_cell,"table-cell");
_LIT(value_type,"value-type");
_LIT(table_cell_properties,"table-cell-properties");
_LIT(number_rows_repeated,"number-rows-repeated");
_LIT(number_columns_repeated,"number-columns-repeated");
_LIT(number_columns_spanned,"number-columns-spanned");

// draw
_LIT(page,"page");
_LIT(image,"image");

_LIT(draw,"draw");
_LIT(width,"width");
_LIT(height,"height");
_LIT(frame,"frame");



//text
_LIT(p,"p");
_LIT(h,"h");
_LIT(span,"span");
_LIT(tab,"tab");
_LIT(s,"s");
_LIT(line_break,"line-break");

_LIT(a,"a"); ////?????

_LIT(list,"list");
_LIT(list_item,"list-item");
_LIT(list_style,"list-style");
_LIT(start_value,"start-value");
_LIT(list_level_style_number,"list-level-style-number");
_LIT(list_level_style_bullet,"list-level-style-bullet");
_LIT(list_level_style_image,"list-level-style-image");
_LIT(list_level_properties,"list-level-properties");
_LIT(space_before,"space-before");
_LIT(bullet_char,"bullet-char");
_LIT(num_format,"num-format");

_LIT(note_body,"note-body");
_LIT(note_citation,"note-citation");

//changes
_LIT(change_start,"change-start");
_LIT(change_end,"change-end");
_LIT(tracked_changes,"tracked-changes");

//meta
_LIT(meta,"meta");
_LIT(dc,"dc");
_LIT(description,"description");
_LIT(title,"title");
_LIT(subject,"subject");
_LIT(document_statistic,"document-statistic");
_LIT(page_count,"page-count");
_LIT(paragraph_count,"paragraph-count");
_LIT(word_count,"word-count");
_LIT(keyword,"keyword");


//Misc
_LIT(footer,"footer");
_LIT(header,"header");
_LIT(annotation,"annotation");

_LIT(note_addition,") ");

// File-entries
_LIT(manifest,"manifest");
_LIT(file_entry,"file-entry");
_LIT(full_path,"full-path");
_LIT(file_size,"size");

// Encryption data
_LIT(encryption_data,"encryption-data");
_LIT(algorithm,"algorithm");
_LIT(key_derivation,"key-derivation");
_LIT(checksum,"checksum");
_LIT(initialisation_vector,"initialisation-vector");
_LIT(salt,"salt");
	


/***********************writing******************/


_LIT(start_xml,	"<office:document-content xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
				" xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\""
				" xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\"" 
				" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
				" xmlns:draw=\"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0\""
				" xmlns:fo=\"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0\""
				" xmlns:xlink=\"http://www.w3.org/1999/xlink\""
				" xmlns:dc=\"http://purl.org/dc/elements/1.1/\""
				" xmlns:meta=\"urn:oasis:names:tc:opendocument:xmlns:meta:1.0\""
				" xmlns:number=\"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0\""
				" xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\"" 
				" xmlns:chart=\"urn:oasis:names:tc:opendocument:xmlns:chart:1.0\""
				" xmlns:dr3d=\"urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0\""
				" xmlns:math=\"http://www.w3.org/1998/Math/MathML\""
				" xmlns:form=\"urn:oasis:names:tc:opendocument:xmlns:form:1.0\""
				" xmlns:script=\"urn:oasis:names:tc:opendocument:xmlns:script:1.0\""
				" xmlns:ooo=\"http://openoffice.org/2004/office\""
				" xmlns:ooow=\"http://openoffice.org/2004/writer\""
				" xmlns:oooc=\"http://openoffice.org/2004/calc\""
				" xmlns:dom=\"http://www.w3.org/2001/xml-events\""
				" xmlns:xforms=\"http://www.w3.org/2002/xforms\"" 
				" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\""
				" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\""
				" office:version=\"1.0\">");


_LIT(start_xml_xml,	"<office:document-styles xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\""
				" xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\""
				" xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\"" 
				" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""
				" xmlns:draw=\"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0\""
				" xmlns:fo=\"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0\""
				" xmlns:xlink=\"http://www.w3.org/1999/xlink\""
				" xmlns:dc=\"http://purl.org/dc/elements/1.1/\""
				" xmlns:meta=\"urn:oasis:names:tc:opendocument:xmlns:meta:1.0\""
				" xmlns:number=\"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0\""
				" xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\"" 
				" xmlns:chart=\"urn:oasis:names:tc:opendocument:xmlns:chart:1.0\""
				" xmlns:dr3d=\"urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0\""
				" xmlns:math=\"http://www.w3.org/1998/Math/MathML\""
				" xmlns:form=\"urn:oasis:names:tc:opendocument:xmlns:form:1.0\""
				" xmlns:script=\"urn:oasis:names:tc:opendocument:xmlns:script:1.0\""
				" xmlns:ooo=\"http://openoffice.org/2004/office\""
				" xmlns:ooow=\"http://openoffice.org/2004/writer\""
				" xmlns:oooc=\"http://openoffice.org/2004/calc\""
				" xmlns:dom=\"http://www.w3.org/2001/xml-events\""
				" xmlns:xforms=\"http://www.w3.org/2002/xforms\"" 
				" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\""
				" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\""
				" office:version=\"1.0\">");


_LIT(start_xml_style,"<office:automatic-styles><style:page-layout style:name=\"pm1\">"
					"<style:header-style/>"
					"<style:footer-style/>"
					"</style:page-layout>"
					"</office:automatic-styles><office:master-styles>"
					"<style:master-page style:name=\"Standard\" style:page-layout-name=\"pm1\"/>"
					"</office:master-styles>"
					"</office:document-styles>");


_LIT(start_xml_writer,	"<office:body>"
						"<office:text>");
_LIT(end_xml_writer,	"</office:text>");
_LIT(end_xml,"</office:body></office:document-content>");

_LIT(start_xml_para,"<text:p text:style-name=\"Standard\">");
_LIT(end_xml_para,"</text:p>");
_LIT(startend_xml_para,"<text:p/>");



_LIT(xml_manifest,	"<manifest:manifest xmlns:manifest=\"urn:oasis:names:tc:opendocument:xmlns:manifest:1.0\">"
					"<manifest:file-entry manifest:media-type=\"application/vnd.oasis.opendocument.text\" manifest:full-path=\"/\"/>"
					"<manifest:file-entry manifest:media-type=\"text/xml\" manifest:full-path=\"content.xml\"/>"
					"<manifest:file-entry manifest:media-type=\"text/xml\" manifest:full-path=\"styles.xml\"/>"
					"</manifest:manifest>");

#endif

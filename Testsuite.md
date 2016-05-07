# Introduction #

To get a first overview of the completness of Office Reader, it was tested using this ODF "testsuite": http://develop.opendocumentfellowship.com/testsuite/

Please note, that office reader is not a desktop application, but an application for mobile devices. So in a lot of cases where a feature is not supported and the test fails, it would not even make sense to implement this on a mobile device. A lot of tests fail as well, because the Symbian Editor component does not support this feature.

# Details #

## General ##

| **Group Name** | **Test Name** | **Pass** |
|:---------------|:--------------|:---------|
| Document Structure | Predefined Metadata |          |
|                | User defined Metadata |          |
|                | Multiple Document Contents | x        |
|                | SingleDocumentContents | x        |


## Formatting Properties ##

| **Group Name** | **Test Name** | **Pass**|
|:---------------|:--------------|:--------|
| ParagraphFormattingProperties | automaticTextIndent |         |
|                | border        | x       |
|                | borderLineWidth |         |
|                | Break before and break after |         |
|                | delimiterCharacter | x       |
|                | Drop caps distance |         |
|                | Drop caps length |         |
|                | dropCapsLines |         |
|                | dropCapsTextStyle |         |
|                | Fixed line height |         |
|                | Font-independent line spacing |         |
|                | justifySingleWord |         |
|                | Leader color  |         |
|                | leaderStyle   |         |
|                | leaderText    |         |
|                | leaderTextStyle |         |
|                | leaderType    |         |
|                | leaderWidth   |         |
|                | Left and right margins | x       |
|                | Line distance |         |
|                | Line number start value |         |
|                | Line numbering |         |
|                | Margins       |         |
|                | minimumLineHeight |         |
|                | Orphans       |         |
|                | padding       |         |
|                | paragraph Background Color | x       |
|                | paragraphBackgroundImageOpacity |         |
|                | Paragraph background image position |         |
|                | paragraphBackgroundImageRepetition |         |
|                | punctuationWrap |         |
|                | Register true |         |
|                | shadow        |         |
|                | Tab Position  |         |
|                | tabStopDistance |         |
|                | Tab type      |         |
|                | textAlign     | x       |
|                | Text align of last line |         |
|                | Text autospace |         |
|                | Widows        |         |
|                | textIndent    | x       |
|                | Top and Bottom Margins | x       |
|                | Vertical Alignment |         |
|                | writingMode   |         |
|                | keepTogether  |         |
| TextFormattingProperties | color         | x       |
|                | country       |         |
|                | Font character set |         |
|                | fontFamily    |         |
|                | fontFamilyGeneric |         |
|                | fontName      |         |
|                | fontPitch     |         |
|                | fontRelief    |         |
|                | fontSize      | x       |
|                | fontStyle     | x       |
|                | fontVariant   |         |
|                | Font Weight   | x       |
|                | Hidden or conditional text |         |
|                | hyphenation   |         |
|                | hyphenationPushCharCount |         |
|                | hyphenationRemainCharCount |         |
|                | language      |         |
|                | letterKerning |         |
|                | letterSpacing |         |
|                | lineThroughColor |         |
|                | lineThroughStyle |         |
|                | lineThroughText |         |
|                | lineThroughTextStyle |         |
|                | lineThrough Width |         |
|                | relativeFontSize |         |
|                | scriptType    |         |
|                | textBackgroundColor | x       |
|                | textBlinking  |         |
|                | textCombine   |         |
|                | textCombineStartAndEndCharacters |         |
|                | textEmphasis  |         |
|                | textLineThroughWordMode |         |
|                | textLineTroughWordMode |         |
|                | textOutline   |         |
|                | textPosition  |         |
|                | textRotationAngle |         |
|                | Text Rotation and Scale |         |
|                | textScale     |         |
|                | Text Shadow   |         |
|                | textTransformations |         |
|                | textUnderlineWordMode |         |
|                | underlineColor |         |
|                | underliningStyle | x       |
|                | underliningType | x       |
|                | underliningWidth |         |
|                | windowFontColor |         |
|                | Line through type | x       |


## Text Contents ##

| **Group Name** | **Test Name** | **Pass** |
|:---------------|:--------------|:---------|
| Lists          | bulletedList  | x        |
|                | continueNumbering | x        |
|                | embededBulletedList | x        |
|                | listHeader    | x        |
|                | numberedList  | x        |
|                | startValue    | x        |
| Headings       | Headings      | x        |
| Paragraph      | Bold and italic text in the middle of a paragraph | x        |
|                | BasicContents | x        |
| textFormatting | Alignment     | x        |
|                | fontColors    | x        |
|                | fontSize      |x         |
|                | Bold and Italic | x        |

## Results ##

| **Section** | **Total** | **Pass** | **Fail** | **%** |
|:------------|:----------|:---------|:---------|:------|
| General     | 4         | 2        | 2        | 50    |
| Formatting Properties | 90        | 15       | 75       | 17    |
| Text Contents | 13        | 13       | 0        | 100   |

ColumnFormattingProperties, TableFormattingProperties, TableCellFormattingProperties, TableRowFormattingProperties, Text Frames, Drawing Content and Presentation content testcases were not checked, as Office Reader does not support this completly currently.
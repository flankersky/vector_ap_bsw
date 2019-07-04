cmake_minimum_required(VERSION 3.4)

# LATEX Configuration
set(TOPLEVEL_TEX_FILE TechnicalReference_per)
set(TOPLEVEL_TEX_PATH ${CMAKE_CURRENT_LIST_DIR}/${TOPLEVEL_TEX_FILE}.tex)
set(STYLE_NAME ${CMAKE_CURRENT_LIST_DIR}/Style/Style_TechnicalReference.ist)
set(STYLE_SVN_URI https://vglobpessvn1.vg.vector.int/svn/Products/AdaptiveMICROSAR/AMSR_Product/trunk/_doc/30_Documentation/TechnicalReferenceLatex/Style)
set(STYLE_SVN_REVISION 33028)
set(FIGURES_SVN_URI https://vglobpessvn1.vg.vector.int/svn/CANbedded/SysService/SysService_AsrApExec/base/Doc_TechRef/trunk/10_Figures)
set(FIGURES_SVN_REVISION 336295)
# Environment variable which describes pdflatex input search path.
set(TEXINPUTS TEXINPUTS=${CMAKE_CURRENT_LIST_DIR}//:${CMAKE_BINARY_DIR}/doc/doxygen/latex//:) # use current directory and doxygen latex output

# LATEX commands
set(PDFLATEX ${TEXINPUTS} pdflatex -synctex=1 -interaction=nonstopmode -file-line-error -jobname=${TOPLEVEL_TEX_FILE} -halt-on-error ${TOPLEVEL_TEX_PATH})
set(BIBTEX bibtex ${TOPLEVEL_TEX_FILE})
set(MAKEINDEX1 makeindex -s ${STYLE_NAME} ${TOPLEVEL_TEX_FILE}.idx)
set(MAKEINDEX2 makeindex -q ${TOPLEVEL_TEX_FILE}.nlo -o ${TOPLEVEL_TEX_FILE}.nls)

add_custom_target(techRef
  #ALL # Enable this line to add this target to the ALL target.
  COMMAND svn checkout ${STYLE_SVN_URI} --revision ${STYLE_SVN_REVISION}
  #COMMAND mv Style/ 00_Style/
  COMMAND svn checkout ${FIGURES_SVN_URI} --revision ${FIGURES_SVN_REVISION}
  COMMAND ${PDFLATEX}
  COMMAND ${BIBTEX}
  COMMAND ${MAKEINDEX1}
  COMMAND ${MAKEINDEX2}
  COMMAND ${PDFLATEX}
  COMMAND ${MAKEINDEX1}
  COMMAND ${MAKEINDEX2}
  COMMAND ${PDFLATEX}
  COMMAND ${MAKEINDEX1}
  COMMAND ${MAKEINDEX2}
  COMMAND ${PDFLATEX}
  COMMAND mv ${TOPLEVEL_TEX_FILE}.pdf ${CMAKE_BINARY_DIR}/doc/TechRef.pdf
  COMMAND ${CMAKE_CURRENT_LIST_DIR}/clean_latex.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  COMMENT "Generating TechRef documentation"
  VERBATIM
)

# Doxygen is always built before TechRef is built.
add_dependencies(techRef documentation)


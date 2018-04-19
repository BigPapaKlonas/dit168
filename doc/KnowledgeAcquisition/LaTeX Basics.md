### Syntax
1) Start the document with "\documentclass[12pt]{article}" this creates a "word" document with font size 12

2) After this you can import packages that you need by typing \usepackage{PackageName}. For example in SAD we use \usepackage{graphicx} to import images, the \usepackage{indentfirst} indents the first paragraph in the section, since by default this is off in LaTeX. There are other packages we use, if add a comment in the SAD document why we have them.

3) Now that you have the import you can start the document with "\begin{document}", you must also use "\end{document}" at the very end of the document. So pretty much all the text, graphics etc are gonna be encloded within these two commands. The {} are used to pass a name or a parameter, so you can use the "\begin" and "\end" commands for other stuff, like for example figures or longtables.

Syntax wise this is pretty much it. LaTeX is commonly used, so if you need anything you could google for it like any other code. 

A good site to refer to for help -> https://www.sharelatex.com/learn or https://tex.stackexchange.com/


### Editors
1) The best that I now is Overleaf, it provides autocomplete, autocompile and can share your project with other people. It also provides a project window, where you can see the project layout, so you can create new folders and import images. install tuturial for ubuntu -> https://milq.github.io/install-latex-ubuntu-debian/ 

3) Gummi, supposedly lightweight and it provides autoCOMPILE, unfortunatelly it does NOT have autoCOMPLETE. Since there are a lot of commands in LaTeX, it might be best to use something with autocomplete like Overleaf. It's also possible to leave comments for other writers. Unfortunatelly, it requires an internet connection.

2) TexMaker is a popular editor as well. I don't now much about this one, since I use the third option, but here's the install tuturial for ubuntu -> https://milq.github.io/install-latex-ubuntu-debian/ 

3) Gummi, supposedly lightweight and it provides autoCOMPILE, unfortunatelly it does NOT have autoCOMPLETE. Since there are a lot of commands in LaTeX, it might be best to use something with autocomplete like Overleaf.


### Sections
\section{SectionName} -> creates a new text section. You can you start typing under it. In Latex by default the first paragraph will not be indented so we use a package to change that. We also use "\setlength{\parindent}{20pt}" to set the indent value for all paragraphs to 20 (something... not sure what pt is). If you write multiple paragraphs or just 1 giant block of text and they arent indented or turned to paragraphs you can use the command "\par" to fix that


### Lists

Good link about lists -> https://www.sharelatex.com/learn/Lists

1) To make a list you are gonna need to use "\begin{enumerate}" and "\end{enumerate}". This is gonna create an empty list with no item. To add stuff to the list use "\item Here's the list item text", so the simplest list would look like:
"\begin{enumerate}
  \item Item 1
\end{enumerate}"

2) You can nest lists, to create to index the items. This thing is gonna make Item 1 have, a) and b) sub lines (lists?). As I understand, the nesting can have depth of 4.
"\begin{enumerate}
  \item Item 1
  \begin{enumerate}
    \item a
    \item b
  \end{enumerate}
\end{enumerate}"

3) Some other useful commands that we use in SAD
\setcounter{enumi}{X} % -> Is used to change the numbering in a list. It starts list from X + 1, so if we use "\setcounter{enumi}{3}", the first item in the list will starts with index 4

\itemsep Xem % -> Change X to specify gap size between numbers in list, by default it's pretty big, to I set "\itemsep 0em" to make them smaller. X could also be a negative value 
         		
       	
### Tables
We use a package called longtables, this lets the table span multiple pages, so the table wraps to the next page if theres not enough room.

1) To create table you use the \begin command. BUT, you need a SECOND set of {}, to specify the number of column and their allignment. For example, "\begin{longtable}{ | r | r | }" will create a table with two columns alligned to the right. The allignment can be r (for right), c (for center) and l (for left). You can also combine the leter, but I'm not sure of the effect. The problem is by using these allignments, we create a static table. Meaning the text won't wrap, so if the leftmost column has long enough text it could occupy the whole line and the right most column would be pushed out. We use  "p{0.55\linewidth}" instead of the r,c or l to specify that this column should take of (in this case) 55% of the text line. This makes the text wrap and prevents the column from expanding. For example:

\begin{longtable} {| p{0.45\linewidth} | p{0.55\linewidth} |} \hline (\hline creates a horizontal line)
\end{longtable}


2) Pipe symbol.
I use the pipe symbol between the coulumn allignments, to tell Latex that there needs to be a line between the two. The pipe symbol corresponds to a horizontal line.

3) Adding stuff to table

So once you can crate a table you can add values to it, like for example:
\begin{longtable} {| p{0.45\linewidth} | p{0.55\linewidth} |} \hline (\hline creates a horizontal line)
  Name of Column 1 & Name of Column 2 \\ \hline
  Column 1 value & Column 3 value \\ \hline
\end{longtable}

The "&" is used to seperate the values into columns, the "\\" tells Latex that this is a new line and \hline draws a horizontal line

Figures
Import \usepackage{graphicx} to use graphics. We use "\usepackage{placeins}" to help position the images. By default in Latex the text goes in front of the image if there is room for text, this is a problem if we want to use the image, for example, in section 5.3, but since there's extra space, so section 5.4 wraps in front of the image. The placeins make it possible to stop this using a "\FloatBarrier" command (by enclosing the figure). I do not fully grasp why the text wraps in front of images, supposedly it has to to with floats. But here how we fix it for example

% Adding image
\FloatBarrier %     -> Wrap image with this, to make sure text does not go infront of image if theres room
\begin{figure}[ht!] -> ht! means stay here (for h) at the top (for t)
\includegraphics[width=\linewidth]{Diagrams/ConnectAsLeader.png} -> the images are in Architecture/Diagrams
\caption{Connect As Leader SD}  -> caption of the image
\label{fig:stopfollowing}       -> label used to refer to image
\end{figure}
\FloatBarrier % -> Wrap image with this, to make sure text does not go infront of image if theres room

Here's how we reference the image "depicted in Figure \ref{fig:connectasfollower}" -> You have to reference the label





<TeXmacs|1.99.2>

<style|<tuple|old-generic|maxima|french>>

<\body>
  <\session|maxima|default>
    <\output>
      \;

      Maxima 5.27.0 http://maxima.sourceforge.net

      using Lisp GNU Common Lisp (GCL) GCL 2.6.7 (a.k.a. GCL)

      Distributed under the GNU Public License. See the file COPYING.

      Dedicated to the memory of William Schelter.

      The function bug_report() provides bug reporting information.
    </output>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>1) >
    <|unfolded-io>
      F:matrix([F0,F3/sqrt(2),0],[F3/sqrt(2),F1,0],[0,0,F2])
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o1>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F0>>|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|F3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|F1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|F2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>2) >
    <|unfolded-io>
      G:matrix([G0,G3/sqrt(2),0],[G3/sqrt(2),G1,0],[0,0,G2])
    <|unfolded-io>
      \;

      \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o2>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|G0>>|<cell|<frac|<with|math-font-family|rm|G3>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|G3>|<sqrt|2>>>|<cell|<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>3) >
    <|unfolded-io>
      prod:F.G+G.F
    <|unfolded-io>
      \;

      \ 

      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o3>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>+2*<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>|<cell|<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G3>|<sqrt|2>>+<frac|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>|<sqrt|2>>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G1>|<sqrt|2>>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G0>|<sqrt|2>>>|<cell|0>>|<row|<cell|<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G3>|<sqrt|2>>+<frac|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>|<sqrt|2>>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G1>|<sqrt|2>>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G0>|<sqrt|2>>>|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>+2*<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>|<cell|0>>|<row|<cell|0>|<cell|0>|<cell|2*<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>4) >
    <|unfolded-io>
      pv:transpose(matrix([prod[1,1],prod[2,2],prod[3,3],sqrt(2)*prod[1,2]]))
    <|unfolded-io>
      <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o4>)
      >><matrix|<tformat|<table|<row|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>+2*<with|math-font-family|rm|F0>*<with|math-font-family|rm|G0>>>|<row|<cell|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G3>+2*<with|math-font-family|rm|F1>*<with|math-font-family|rm|G1>>>|<row|<cell|2*<with|math-font-family|rm|F2>*<with|math-font-family|rm|G2>>>|<row|<cell|<sqrt|2>*<around*|(|<frac|<with|math-font-family|rm|F1>*<with|math-font-family|rm|G3>|<sqrt|2>>+<frac|<with|math-font-family|rm|F0>*<with|math-font-family|rm|G3>|<sqrt|2>>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G1>|<sqrt|2>>+<frac|<with|math-font-family|rm|F3>*<with|math-font-family|rm|G0>|<sqrt|2>>|)>>>>>>>>
    </unfolded-io>

    <\unfolded-io>
      <with|color|red|(<with|math-font-family|rm|%i>7) >
    <|unfolded-io>
      J1:fullratsimp(addcol(diff(pv,F0),diff(pv,F1),diff(pv,F2),diff(pv,F3)))
    <|unfolded-io>
      \;

      \ <math|<with|math-display|true|<text|<with|font-family|tt|color|red|(<with|math-font-family|rm|%o7>)
      >><matrix|<tformat|<table|<row|<cell|2*<with|math-font-family|rm|G0>>|<cell|0>|<cell|0>|<cell|<with|math-font-family|rm|G3>>>|<row|<cell|0>|<cell|2*<with|math-font-family|rm|G1>>|<cell|0>|<cell|<with|math-font-family|rm|G3>>>|<row|<cell|0>|<cell|0>|<cell|2*<with|math-font-family|rm|G2>>|<cell|0>>|<row|<cell|<with|math-font-family|rm|G3>>|<cell|<with|math-font-family|rm|G3>>|<cell|0>|<cell|<with|math-font-family|rm|G1>+<with|math-font-family|rm|G0>>>>>>>>
    </unfolded-io>

    <\input>
      <with|color|red|(<with|math-font-family|rm|%i>8) >
    <|input>
      \;
    </input>

    \;
  </session>
</body>

<\initial>
  <\collection>
    <associate|page-type|a2>
  </collection>
</initial>
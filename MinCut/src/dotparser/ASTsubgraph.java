/* Generated By:JJTree: Do not edit this line. ASTsubgraph.java Version 4.3 */
/* JavaCCOptions:MULTI=true,NODE_USES_PARSER=false,VISITOR=true,TRACK_TOKENS=false,NODE_PREFIX=AST,NODE_EXTENDS=,NODE_FACTORY=,SUPPORT_CLASS_VISIBILITY_PUBLIC=true */
package dotparser;

public
class ASTsubgraph extends SimpleNode {
  public ASTsubgraph(int id) {
    super(id);
  }

  public ASTsubgraph(DOTParser p, int id) {
    super(p, id);
  }


  /** Accept the visitor. **/
  public Object jjtAccept(DOTParserVisitor visitor, Object data) {
    return visitor.visit(this, data);
  }
}
/* JavaCC - OriginalChecksum=0ca8c68dce8e86eaab81383b649abbc6 (do not edit this line) */
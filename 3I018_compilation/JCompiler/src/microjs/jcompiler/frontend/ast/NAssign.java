package microjs.jcompiler.frontend.ast;

import java.util.ArrayList;
import java.util.List;

import java_cup.runtime.ComplexSymbolFactory.Location;
import microjs.jcompiler.middleend.kast.*;
import microjs.jcompiler.utils.DotGraph;

public class NAssign extends Statement {
    private String name;
    private String operator;
    private Expr   expr;
    
    public NAssign(String name, String operator, Expr expr,
		   Location startPos, Location endPos) {
    	super(startPos, endPos);		
    	this.name     = name;
	this.operator = operator;
	this.expr     = expr;
    }

    @Override
    public KAssign expand() {
	List<KExpr> args = new ArrayList<KExpr>();

	args.add(new KEVar(name, getStartPos(), getEndPos()));
	args.add(expr.expand());

    	return new KAssign(name,
			   new KCall(new KEVar(operator.substring(0, 1),
					       getStartPos(), getEndPos()),
			       args, getStartPos(), getEndPos())
			   , getStartPos(), getEndPos());
    }

    @Override
    protected void prettyPrint(StringBuilder buf, int indent_level) {
    	indent(buf, indent_level);
    	buf.append(name);
    	buf.append(" " + operator + " ");
    	expr.prettyPrint(buf);
    }
    
    @Override
    protected String buildDotGraph(DotGraph graph) {
	String assignNode = graph.addNode("NAssign[" + name + ", "+
						  operator + "]");
	String exprNode = expr.buildDotGraph(graph);
	graph.addEdge(assignNode, exprNode, "expr");
	
	return assignNode;
    }
}

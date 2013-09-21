<?php

require_once('Token.class.inc');

class Tokenizer
{
    private $pos; // position of next character (unprocessed)
    private $file;
    private $len;
    private $lineNum;
    
    public $tokens; // token array
    private $currentTokenIndex;
    
    function __construct($pFile)
    {
        $this->file = $pFile;
        $this->removeComments();
        $this->len = strlen($this->file);
        
        $this->tokenize();
        $this->goToStart();
    }
    
    function goToStart()
    {
        $this->currentTokenIndex = 0;
    }
    
    function getNextToken()
    {   
        if ($this->currentTokenIndex == count($this->tokens) - 1) return false;
        return $this->tokens[$this->currentTokenIndex++];
    }
    
    function backUpOne()
    {
        $this->currentTokenIndex--;
    }
    
    
    //      ____________
    //_____/  private   \__________________
    
    private function tokenize()
    {
        $this->pos = 0;
        $this->lineNum = 1;
        
        $this->tokens = array();
        $token = false;
        do
        {
            $token = $this->fetchToken();
            $this->tokens[] = $token;
        } while ($token !== false);
    }
    
    /*
        Returns the content of the next token (with surrounding whitespace removed)
    */
    private function fetchTokenText()
    {
        // Start at next non-whitespace character
        while ($this->pos != $this->len
            && self::isWhitespace($this->char()))
        {
            $this->checkNewLine();
            $this->pos++;
        }
        
        if ($this->pos == strlen($this->file)) return false; // reached end of file
        
        
        // Find end of token
        $tokenStartPos = $this->pos;
        
        if ($this->char() == '@')
        {
            // keyword token - find next whitespace
            do
            {
                $this->checkNewLine();
                $this->pos++;
            } while ($this->pos != $this->len
                && !self::isWhitespace($this->file[$this->pos])
                && $this->char() != '@');
        }
        else
        {
            // string token - find start of next keyword
            do
            {
                $this->checkNewLine();
                $this->pos++;
            } while ($this->pos != $this->len
                && !($this->char() == '@' && $this->file[$this->pos-1] != '\\'));
        }
        
        $tokenText = trim(substr($this->file, $tokenStartPos, $this->pos-$tokenStartPos));
        $tokenText = str_replace('\\@', '@', $tokenText);
        return $tokenText;
    }
    
    private function fetchToken()
    {
        $tokenText = $this->fetchTokenText();
        if ($tokenText == false) return false;
        
        return new Token($tokenText, $this->lineNum);
    }
    
    private static function isWhitespace($pChar)
    {
        return ($pChar == ' ' || $pChar == "\n" || $pChar == "\r" || $pChar == "\t");
    }
    
    private function char()
    {
        return $this->file[$this->pos];
    }
    
    private function checkNewLine()
    {
        if ($this->char() == "\n") $this->lineNum++;
    }
    
    /**
        Removes comments and replaces \-escaped #s with a single #
    **/
    private function removeComments()
    {
        $commentPos = 0;
        do
        {
            $commentPos = strpos($this->file, '#', $commentPos);
            if ($commentPos !== false)
            {
                // normal # comment
                if ($commentPos != 0 && $this->file[$commentPos-1] != '\\')
                {
                    $newlinePos = strpos($this->file, "\n", $commentPos);
                    $this->file = substr($this->file, 0, $commentPos)
                        . substr($this->file, $newlinePos);
                }
                else
                {
                    // escaped #
                    // remove escape character
                    $this->file = substr($this->file, 0, $commentPos-1)
                        . substr($this->file, $commentPos);
                }
            }
        }
        while ($commentPos !== false);
    }
   
}
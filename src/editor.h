/**
* Contains: Editor
*
* This is a basic editor object
 */
#ifndef EDITOR_H
#define EDITOR_H
#include <vector>
#include <string>
#include "mud.h"
#include "player.h"
#include "socket.h"
#include "event.h"
#include "inputHandlers.h"

/**
* The editor is an interface to edit buffers.
*/
class Editor
{
protected:
    Player* _mobile;
    std::vector <std::string> _lines;
    int _cursor;
    BOOL _dirty;
    void* _arg;
public:
    EventManager events;

    Editor();
    ~Editor() = default;

    /**
    * Called to fire the loaded event.
    */
    virtual BOOL Load();
    /**
    * Called to fire the saved event and change editor state.
    */
    virtual void Save();
    /**
    * Trashes the editor buffer and leaves the editor.
    */
    virtual void Abort();
    /**
    * Quits the editor if the buffer has been saved.
    */
    virtual void Quit();

    /**
    * Prints a list of the lines in the buffer.
    *
    * @param [in] num True to number the lines of output, false otherwise.
    */
    virtual void List(BOOL lnum);
    /**
    * Adds the specified line to the editor.
    *
    * The line will be added where the cursor is or at the end of the buffer.
    * @param [in] line the line to add.
    * @param [in] quiet Whether or not to announce the line was added (set this to true if you are populating a buffer).
    */
    virtual void Add(const std::string &line, BOOL quiet = false);
    /**
    * Sets the insertion point of the editor.
    *
    * @param [in] index the position on which to set the cursor.
    */
    virtual void Insert(int index);
    /**
    * Deletes the line at the cursor.
    */
    virtual void Delete();
    /**
    * Deletes the line at the specified index.
    *
    * @param [in] index the position to delete.
    */
    virtual void Delete(int index);
    /**
    * Deletes a range of lines.
    *
    * @param [in] first the beginning of the region to delete.
    * @param [in] second the end of the region to delete.
    */
    virtual void Delete(int first, int second);

    /**
    * Initializes the editor for the player.
    */
    virtual BOOL EnterEditor(Player* mobile);
    /**
    * Removes the player from the editor and clears the input handler.
    */
    virtual void LeaveEditor();

    /**
    * Sets the argument attached to the editor.
    *
    * Use this to attach extra data to the editor.
    * @param [in] arg the argument to attach.
    */
    void SetArg(void* arg);
    \
    /**
    * Returns associated data with the editor.
    */
    void* GetArg() const;
    /**
    * Returns a pointer to a vector of strings associated with this editor.
    */
    std::vector<std::string>* GetLines();

    /**
    * The editor input callback.
    *
    * Parses arguments and editor commands.
    * @todo fix to behave more like ed.
    * @param [in] arg the argument attached to the editor.
    * @param [in] input the input from the socket.
    */
    void Input(void* arg, const std::string &input);
    /**
    * Input handler for the text block.
    */
    void TextInput(Socket* sock, std::vector<std::string>* lines, void* args);
};
#endif

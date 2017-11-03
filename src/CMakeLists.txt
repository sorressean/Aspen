find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)

add_executable(aspen
world.cpp variant.cpp baseObject.cpp entity.cpp player.cpp
socket.cpp utils.cpp log.cpp server.cpp main.cpp
exception.cpp command.cpp com_gen.cpp com_wiz.cpp event.cpp
delayedEvent.cpp eventManager.cpp staticObject.cpp living.cpp channel.cpp
modules.cpp component.cpp npc.cpp editor.cpp exit.cpp
room.cpp zone.cpp com_builder.cpp com_movement.cpp ComponentFactory.cpp
alias.cpp olc.cpp socials.cpp olcs.cpp callout.cpp
option.cpp baseSocket.cpp banList.cpp listener.cpp inputHandlers.cpp
property.cpp uuid.cpp olcGroup.cpp olcManager.cpp playerManager.cpp
match.cpp calloutManager.cpp optionManager.cpp objectContainer.cpp telnetParser.cpp
optionMeta.cpp bitfield.cpp attribute.cpp affect.cpp objectManager.cpp
page.cpp serializationHelpers.cpp
extensions/extensions.cpp
extensions/utils.cpp
modules/modules.cpp
modules/board/board.cpp
modules/board/boardManager.cpp
modules/board/boardModule.cpp
modules/board/boardPost.cpp
syslog/syslog.cpp
scripts/scr_BaseObject.cpp
scripts/scr_Entity.cpp
scripts/scr_Exit.cpp
scripts/scr_ObjectContainer.cpp
scripts/scr_Room.cpp
scripts/script.cpp
)

target_link_libraries (aspen LINK_PUBLIC
angelscript
tinyxml
${ZLIB_LIBRARIES}
${OPENSSL_SSL_LIBRARY}
${OPENSSL_CRYPTO_LIBRARY}
${Boost_LIBRARIES}
)

    include_directories( ${ZLIB_INCLUDE_DIRS} 
${OpenSSL_INCLUDE_DIRS}
${Boost_INCLUDE_DIRS} 
)

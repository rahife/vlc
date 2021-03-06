/*****************************************************************************
 * vlc_media_library.h: SQL-based media library
 *****************************************************************************
 * Copyright (C) 2008-2010 the VideoLAN Team and AUTHORS
 * $Id$
 *
 * Authors: Antoine Lejeune <phytos@videolan.org>
 *          Jean-Philippe André <jpeg@videolan.org>
 *          Rémi Duraffort <ivoire@videolan.org>
 *          Adrien Maglo <magsoft@videolan.org>
 *          Srikanth Raju <srikiraju at gmail dot com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef VLC_MEDIA_LIBRARY_H
# define VLC_MEDIA_LIBRARY_H

#include <assert.h>
#include <vlc_common.h>

# ifdef __cplusplus
extern "C" {
# endif

typedef enum vlc_ml_media_type_t
{
    VLC_ML_MEDIA_TYPE_UNKNOWN,
    VLC_ML_MEDIA_TYPE_VIDEO,
    VLC_ML_MEDIA_TYPE_AUDIO,
    VLC_ML_MEDIA_TYPE_EXTERNAL,
    VLC_ML_MEDIA_TYPE_STREAM,
} vlc_ml_media_type_t;

typedef enum vlc_ml_media_subtype_t
{
    VLC_ML_MEDIA_SUBTYPE_UNKNOWN,
    VLC_ML_MEDIA_SUBTYPE_SHOW_EPISODE,
    VLC_ML_MEDIA_SUBTYPE_MOVIE,
    VLC_ML_MEDIA_SUBTYPE_ALBUMTRACK,
} vlc_ml_media_subtype_t;

typedef enum vlc_ml_file_type_t
{
    VLC_ML_FILE_TYPE_UNKNOWN,
    VLC_ML_FILE_TYPE_MAIN,
    VLC_ML_FILE_TYPE_PART,
    VLC_ML_FILE_TYPE_SOUNDTRACK,
    VLC_ML_FILE_TYPE_SUBTITLE,
    VLC_ML_FILE_TYPE_PLAYLIST,
} vlc_ml_file_type_t;

typedef enum vlc_ml_track_type_t
{
    VLC_ML_TRACK_TYPE_UNKNOWN,
    VLC_ML_TRACK_TYPE_VIDEO,
    VLC_ML_TRACK_TYPE_AUDIO,
} vlc_ml_track_type_t;

typedef struct vlc_ml_movie_t
{
    char* psz_summary;
    char* psz_imdb_id;
} vlc_ml_movie_t;

typedef struct vlc_ml_show_episode_t
{
    char* psz_summary;
    char* psz_tvdb_id;
    uint32_t i_episode_nb;
    uint32_t i_season_number;
} vlc_ml_show_episode_t;

typedef struct vlc_ml_show_t
{
    int64_t i_id;
    char* psz_name;
    char* psz_summary;
    char* psz_artwork_mrl;
    char* psz_tvdb_id;
    unsigned int i_release_year;
    uint32_t i_nb_episodes;
    uint32_t i_nb_seasons;
} vlc_ml_show_t;

typedef struct vlc_ml_album_track_t
{
    int64_t i_artist_id;
    int64_t i_album_id;
    int64_t i_genre_id;

    int i_track_nb;
    int i_disc_nb;
} vlc_ml_album_track_t;

typedef struct vlc_ml_label_t
{
    int64_t i_id;
    char* psz_name;
} vlc_ml_label_t;

typedef struct vlc_ml_label_list_t
{
    size_t i_nb_items;
    vlc_ml_label_t p_items[];
} vlc_ml_label_list_t;

typedef struct vlc_ml_file_t
{
    char* psz_mrl;
    vlc_ml_file_type_t i_type;
    bool b_external;
} vlc_ml_file_t;

typedef struct vlc_ml_file_list_t
{
    size_t i_nb_items;
    vlc_ml_file_t p_items[];
} vlc_ml_file_list_t;

typedef struct vlc_ml_media_track_t
{
    char* psz_codec;
    char* psz_language;
    char* psz_description;
    vlc_ml_track_type_t i_type;
    uint32_t i_bitrate;
    union
    {
        struct
        {
            // Audio
            uint32_t i_nbChannels;
            uint32_t i_sampleRate;
        } a;
        struct
        {
            // Video
            uint32_t i_height;
            uint32_t i_width;
            uint32_t i_sarNum;
            uint32_t i_sarDen;
            uint32_t i_fpsNum;
            uint32_t i_fpsDen;
        } v;
    };
} vlc_ml_media_track_t;

typedef struct vlc_ml_media_track_list_t
{
    size_t i_nb_items;
    vlc_ml_media_track_t p_items[];
} vlc_ml_media_track_list_t;

typedef struct vlc_ml_media_t
{
    int64_t i_id;

    vlc_ml_media_type_t i_type;
    vlc_ml_media_subtype_t i_subtype;

    vlc_ml_file_list_t* p_files;
    vlc_ml_media_track_list_t* p_tracks;

    int32_t i_year;
    /* Duration in milliseconds */
    int64_t i_duration;
    uint32_t i_playcount;
    time_t i_last_played_date;
    char* psz_title;

    char* psz_artwork_mrl;
    bool b_is_favorite;

    union
    {
        vlc_ml_show_episode_t show_episode;
        vlc_ml_movie_t movie;
        vlc_ml_album_track_t album_track;
    };
} vlc_ml_media_t;

typedef struct vlc_ml_playlist_t
{
    int64_t i_id;
    char* psz_name;
    uint32_t i_creation_date;
    char* psz_artwork_mrl;
} vlc_ml_playlist_t;

typedef struct vlc_ml_artist_t
{
    int64_t i_id;
    char* psz_name;
    char* psz_shortbio;
    char* psz_artwork_mrl;
    char* psz_mb_id;

    unsigned int i_nb_album;
    unsigned int i_nb_tracks;
} vlc_ml_artist_t;

typedef struct vlc_ml_artist_list_t
{
    size_t i_nb_items;
    vlc_ml_artist_t p_items[];
} vlc_ml_artist_list_t;

typedef struct vlc_ml_album_t {
    int64_t i_id;
    char* psz_title;
    char* psz_summary;
    char* psz_artwork_mrl;
    char* psz_artist;
    int64_t i_artist_id;

    size_t i_nb_tracks;
    unsigned int i_duration;
    unsigned int i_year;
} vlc_ml_album_t;

typedef struct vlc_ml_genre_t
{
    int64_t i_id;
    char* psz_name;
    size_t i_nb_tracks;
} vlc_ml_genre_t;

typedef struct vlc_ml_media_list_t
{
    size_t i_nb_items;
    vlc_ml_media_t p_items[];
} vlc_ml_media_list_t;

typedef struct vlc_ml_album_list_t
{
    size_t i_nb_items;
    vlc_ml_album_t p_items[];
} vlc_ml_album_list_t;

typedef struct vlc_ml_show_list_t
{
    size_t i_nb_items;
    vlc_ml_show_t p_items[];
} vlc_ml_show_list_t;

typedef struct vlc_ml_genre_list_t
{
    size_t i_nb_items;
    vlc_ml_genre_t p_items[];
} vlc_ml_genre_list_t;

typedef struct vlc_ml_playlist_list_t
{
    size_t i_nb_items;
    vlc_ml_playlist_t p_items[];
} vlc_ml_playlist_list_t;

typedef struct vlc_ml_entrypoint_t vlc_ml_entrypoint_t;
struct vlc_ml_entrypoint_t
{
    char* psz_mrl; /**< This entrypoint's MRL. Will be NULL if b_present is false */
    bool b_present; /**< The presence state for this entrypoint. */
    bool b_banned; /**< Will be true if the user required this entrypoint to be excluded */
};

typedef struct vlc_medialibrary_t vlc_medialibrary_t;

typedef enum vlc_ml_sorting_criteria_t
{
    /*
     * Default depends on the entity type:
     * - By track number (and disc number) for album tracks
     * - Alphabetical order for others
     */
    VLC_ML_SORTING_DEFAULT,
    VLC_ML_SORTING_ALPHA,
    VLC_ML_SORTING_DURATION,
    VLC_ML_SORTING_INSERTIONDATE,
    VLC_ML_SORTING_LASTMODIFICATIONDATE,
    VLC_ML_SORTING_RELEASEDATE,
    VLC_ML_SORTING_FILESIZE,
    VLC_ML_SORTING_ARTIST,
    VLC_ML_SORTING_PLAYCOUNT,
    VLC_ML_SORTING_ALBUM,
    VLC_ML_SORTING_FILENAME,
    VLC_ML_SORTING_TRACKNUMBER,
} vlc_ml_sorting_criteria_t;

typedef struct vlc_ml_query_params_t vlc_ml_query_params_t;
struct vlc_ml_query_params_t
{
    const char* psz_pattern;
    uint32_t i_nbResults;
    uint32_t i_offset;
    vlc_ml_sorting_criteria_t i_sort;
    bool b_desc;
};

enum vlc_ml_get_queries
{
    VLC_ML_GET_MEDIA,           /**< arg1: Media    ID; ret: vlc_ml_media_t*    */
    VLC_ML_GET_ALBUM,           /**< arg1: Album    ID; ret: vlc_ml_album_t*    */
    VLC_ML_GET_ARTIST,          /**< arg1: Artist   ID; ret: vlc_ml_artist_t*   */
    VLC_ML_GET_GENRE,           /**< arg1: Genre    ID; ret: vlc_ml_genre_t*    */
    VLC_ML_GET_SHOW,            /**< arg1: Show     ID; ret: vlc_ml_show_t*     */
    VLC_ML_GET_PLAYLIST,        /**< arg1: Playlist ID; ret: vlc_ml_playlist_t* */
};

enum vlc_ml_list_queries
{
    /* General listing: */

    VLC_ML_LIST_VIDEOS,           /**< arg1 (out): vlc_ml_media_list_t**                            */
    VLC_ML_COUNT_VIDEOS,          /**< arg1 (out): size_t*                                          */
    VLC_ML_LIST_AUDIOS,           /**< arg1 (out): vlc_ml_media_list_t**                            */
    VLC_ML_COUNT_AUDIOS,          /**< arg1 (out): size_t*                                          */
    VLC_ML_LIST_ALBUMS,           /**< arg1 (out): vlc_ml_album_list_t**                            */
    VLC_ML_COUNT_ALBUMS,          /**< arg1 (out): size_t*                                          */
    VLC_ML_LIST_GENRES,           /**< arg1 (out): vlc_ml_genre_list_t**                            */
    VLC_ML_COUNT_GENRES,          /**< arg1 (out): size_t*                                          */
    VLC_ML_LIST_ARTISTS,          /**< arg1 bool: includeAll; arg2 (out): vlc_ml_genre_list_t**     */
    VLC_ML_COUNT_ARTISTS,         /**< arg1 bool: includeAll; arg2 (out): size_t*                   */
    VLC_ML_LIST_SHOWS,            /**< arg1 (out): vlc_ml_show_list_t**                             */
    VLC_ML_COUNT_SHOWS,           /**< arg1 (out): size_t*                                          */
    VLC_ML_LIST_PLAYLISTS,        /**< arg1 (out): vlc_ml_playlist_list_t**                         */
    VLC_ML_COUNT_PLAYLISTS,       /**< arg1 (out): size_t*                                          */
    VLC_ML_LIST_HISTORY,          /**< arg1 (out): vlc_ml_media_list_t**                            */
    VLC_ML_LIST_STREAM_HISTORY,   /**< arg1 (out): vlc_ml_media_list_t**                            */

    /* Album specific listings */
    VLC_ML_LIST_ALBUM_TRACKS,     /**< arg1: The album id. arg2 (out): vlc_ml_media_list_t**  */
    VLC_ML_COUNT_ALBUM_TRACKS,    /**< arg1: The album id. arg2 (out): size_t*  */
    VLC_ML_LIST_ALBUM_ARTISTS,    /**< arg1: The album id. arg2 (out): vlc_ml_album_list_t**  */
    VLC_ML_COUNT_ALBUM_ARTISTS,    /**< arg1: The album id. arg2 (out): size_t*  */

    /* Artist specific listings */
    VLC_ML_LIST_ARTIST_ALBUMS,  /**< arg1: The artist id. arg2(out): vlc_ml_album_list_t**    */
    VLC_ML_COUNT_ARTIST_ALBUMS, /**< arg1: The artist id. arg2(out): size_t*              */
    VLC_ML_LIST_ARTIST_TRACKS,  /**< arg1: The artist id. arg2(out): vlc_ml_media_list_t**    */
    VLC_ML_COUNT_ARTIST_TRACKS, /**< arg1: The artist id. arg2(out): size_t*              */

    /* Genre specific listings */
    VLC_ML_LIST_GENRE_ARTISTS,    /**< arg1: genre id;  arg2 (out): vlc_ml_artist_list_t**  */
    VLC_ML_COUNT_GENRE_ARTISTS,   /**< arg1: genre id;  arg2 (out): size_t*             */
    VLC_ML_LIST_GENRE_TRACKS,     /**< arg1: genre id;  arg2 (out): vlc_ml_media_list_t**   */
    VLC_ML_COUNT_GENRE_TRACKS,    /**< arg1: genre id;  arg2 (out): size_t*             */
    VLC_ML_LIST_GENRE_ALBUMS,     /**< arg1: genre id;  arg2 (out): vlc_ml_album_list_t**   */
    VLC_ML_COUNT_GENRE_ALBUMS,    /**< arg1: genre id;  arg2 (out): size_t*             */

    /* Show specific listings */
    VLC_ML_LIST_SHOW_EPISODES,    /**< arg1: show id; arg2(out): vlc_ml_media_list_t**  */
    VLC_ML_COUNT_SHOW_EPISODES,   /**< arg1: show id; arg2(out): size_t*                */

    /* Media specific listings */
    VLC_ML_LIST_MEDIA_LABELS,     /**< arg1: media id;  arg2 (out): vlc_ml_label_list_t**    */
    VLC_ML_COUNT_MEDIA_LABELS,    /**< arg1: media id;  arg2 (out): size_t*              */

    /* Playlist specific listings */
    VLC_ML_LIST_PLAYLIST_MEDIA,   /**< arg1: playlist id; arg2 (out): vlc_ml_media_list_t** */
    VLC_ML_COUNT_PLAYLIST_MEDIA,  /**< arg1: playlist id; arg2 (out): size_t* */

    /* Children entities listing */
    VLC_ML_LIST_MEDIA_OF,         /**< arg1: parent entity type; arg2: parent entity id; arg3(out): ml_media_list_t* */
    VLC_ML_COUNT_MEDIA_OF,        /**< arg1: parent entity type; arg2: parent entity id; arg3(out): size_t* */
    VLC_ML_LIST_ARTISTS_OF,       /**< arg1: parent entity type; arg2: parent entity id; arg3(out): ml_artist_list_t* */
    VLC_ML_COUNT_ARTISTS_OF,      /**< arg1: parent entity type; arg2: parent entity id; arg3(out): size_t* */
    VLC_ML_LIST_ALBUMS_OF,        /**< arg1: parent entity type; arg2: parent entity id; arg3(out): ml_album_list_t* */
    VLC_ML_COUNT_ALBUMS_OF,       /**< arg1: parent entity type; arg2: parent entity id; arg3(out): size_t* */
};

enum vlc_ml_parent_type
{
    VLC_ML_PARENT_ALBUM = 1,
    VLC_ML_PARENT_ARTIST,
    VLC_ML_PARENT_SHOW,
    VLC_ML_PARENT_GENRE,
    VLC_ML_PARENT_PLAYLIST,
};

enum vlc_ml_control
{
    /* Adds a folder to discover through the medialibrary */
    VLC_ML_ADD_FOLDER,              /**< arg1: mrl (const char*)  res: can't fail */
    VLC_ML_REMOVE_FOLDER,           /**< arg1: mrl (const char*)  res: can't fail */
    VLC_ML_BAN_FOLDER,              /**< arg1: mrl (const char*)  res: can't fail */
    VLC_ML_UNBAN_FOLDER,            /**< arg1: mrl (const char*)  res: can't fail */
    VLC_ML_LIST_FOLDERS,            /**< arg1: entrypoints (vlc_ml_entrypoint_t**); arg2: nb results(size_t*), res: can fail */

    /* Pause/resume background operations, such as media discovery & media analysis */
    VLC_ML_PAUSE_BACKGROUND,        /**< no args; can't fail */
    VLC_ML_RESUME_BACKGROUND,       /**< no args; can't fail */

    /* Misc operations */
    VLC_ML_CLEAR_HISTORY,           /**< no args; can't fail */

    /* Create media */
    VLC_ML_NEW_EXTERNAL_MEDIA,      /**< arg1: const char*; arg2(out): vlc_ml_media_t** */
    VLC_ML_NEW_STREAM,              /**< arg1: const char*; arg2(out): vlc_ml_media_t** */

    /* Media management */
    VLC_ML_MEDIA_INCREASE_PLAY_COUNT,       /**< arg1: media id; can fail */
    VLC_ML_MEDIA_GET_MEDIA_PLAYBACK_PREF,   /**< arg1: media id; arg2: vlc_ml_playback_pref; arg3: char**; */
    VLC_ML_MEDIA_SET_MEDIA_PLAYBACK_PREF,   /**< arg1: media id; arg2: vlc_ml_playback_pref; arg3: const char*; */
    VLC_ML_MEDIA_SET_THUMBNAIL,             /**< arg1: media id; arg2: const char*; */
    VLC_ML_MEDIA_ADD_EXTERNAL_MRL,          /**< arg1: media id; arg2: const char*; arg3: type(vlc_ml_file_type_t) */
};

/**
 * User playback settings.
 * All values/units are up to the caller and are not interpreted by the media
 * library.
 * All values are stored and returned as strings.
 * When calling vlc_medialibrary_t::pf_control with vlc_ml_MEDIA_GET_MEDIA_PLAYBACK_PREF,
 * the value will be returned stored in the provided char**. If the preference was
 * not set yet, NULL will be returned.
 * When setting a preference, NULL can be provided as a value to unset it.
 */
enum vlc_ml_playback_pref
{
    VLC_ML_PLAYBACK_PREF_RATING,
    VLC_ML_PLAYBACK_PREF_PROGRESS,
    VLC_ML_PLAYBACK_PREF_SPEED,
    VLC_ML_PLAYBACK_PREF_TITLE,
    VLC_ML_PLAYBACK_PREF_CHAPTER,
    VLC_ML_PLAYBACK_PREF_PROGRAM,
    VLC_ML_PLAYBACK_PREF_SEEN,
    VLC_ML_PLAYBACK_PREF_VIDEO_TRACK,
    VLC_ML_PLAYBACK_PREF_ASPECT_RATIO,
    VLC_ML_PLAYBACK_PREF_ZOOM,
    VLC_ML_PLAYBACK_PREF_CROP,
    VLC_ML_PLAYBACK_PREF_DEINTERLACE,
    VLC_ML_PLAYBACK_PREF_VIDEO_FILTER,
    VLC_ML_PLAYBACK_PREF_AUDIO_TRACK,
    VLC_ML_PLAYBACK_PREF_GAIN,
    VLC_ML_PLAYBACK_PREF_AUDIO_DELAY,
    VLC_ML_PLAYBACK_PREF_SUBTITLE_TRACK,
    VLC_ML_PLAYBACK_PREF_SUBTITLE_DELAY,
    VLC_ML_PLAYBACK_PREF_APP_SPECIFIC,
};

struct vlc_medialibrary_t
{
    struct vlc_common_members obj;

    module_t *p_module;

    void* p_sys;

    int (*pf_control)( vlc_medialibrary_t* p_ml, int i_query, ... );
    /**
     * List some entities from the medialibrary.
     *
     * \param p_ml The medialibrary module instance.
     * \param i_query The type search to be performed. \see vlc_ml_list enumeration
     * \param p_params A pointer to a vlc_ml_query_params_t structure, or NULL for
     * the default parameters (alphabetical ascending sort, no pagination)
     *
     * \return VLC_SUCCESS or an error code
     *
     * Refer to the individual list of vlc_ml_list requests for the additional
     * per-query input/ouput parameters values & types
     */
    int (*pf_list)( vlc_medialibrary_t* p_ml, int i_query,
                    const vlc_ml_query_params_t* p_params, ... );

    /**
     * Get a specific entity by its id.
     *
     * \return The required entity, or a NULL pointer if couldn't be found.
     *
     * Refer to the list of queries for the specific return type
     */
    void* (*pf_get)( vlc_medialibrary_t* p_ml, int i_query, int64_t i_id );
};

vlc_medialibrary_t* libvlc_MlCreate( libvlc_int_t* p_libvlc );
void libvlc_MlRelease( vlc_medialibrary_t* p_ml );

VLC_API vlc_medialibrary_t* vlc_ml_instance_get( vlc_object_t* p_obj ) VLC_USED;
#define vlc_ml_instance_get(x) vlc_ml_instance_get( VLC_OBJECT(x) )

VLC_API void vlc_ml_entrypoints_release( vlc_ml_entrypoint_t* p_list, size_t i_nb_items );

VLC_API void vlc_ml_show_release( vlc_ml_show_t* p_show );
VLC_API void vlc_ml_artist_release( vlc_ml_artist_t* p_artist );
VLC_API void vlc_ml_genre_release( vlc_ml_genre_t* p_genre );
VLC_API void vlc_ml_media_release( vlc_ml_media_t* p_media );
VLC_API void vlc_ml_album_release( vlc_ml_album_t* p_album );
VLC_API void vlc_ml_playlist_release( vlc_ml_playlist_t* p_playlist );

VLC_API void vlc_ml_label_list_release( vlc_ml_label_list_t* p_list );
VLC_API void vlc_ml_file_list_release( vlc_ml_file_list_t* p_list );
VLC_API void vlc_ml_artist_list_release( vlc_ml_artist_list_t* p_list );
VLC_API void vlc_ml_media_list_release( vlc_ml_media_list_t* p_list );
VLC_API void vlc_ml_album_list_release( vlc_ml_album_list_t* p_list );
VLC_API void vlc_ml_show_list_release( vlc_ml_show_list_t* p_list );
VLC_API void vlc_ml_genre_list_release( vlc_ml_genre_list_t* p_list );
VLC_API void vlc_ml_playlist_list_release( vlc_ml_playlist_list_t* p_list );

static inline vlc_ml_query_params_t vlc_ml_query_params_create()
{
    return (vlc_ml_query_params_t) {
        .psz_pattern = NULL,
        .i_nbResults = 0,
        .i_offset = 0,
        .i_sort = VLC_ML_SORTING_DEFAULT,
        .b_desc = false
    };
}

static inline int vlc_ml_add_folder( vlc_medialibrary_t* p_ml, const char* psz_folder )
{
    return p_ml->pf_control( p_ml, VLC_ML_ADD_FOLDER, psz_folder );
}

static inline int vlc_ml_remove_folder( vlc_medialibrary_t* p_ml, const char* psz_folder )
{
    return p_ml->pf_control( p_ml, VLC_ML_REMOVE_FOLDER, psz_folder );
}

static inline int vlc_ml_ban_folder( vlc_medialibrary_t* p_ml, const char* psz_folder )
{
    return p_ml->pf_control( p_ml, VLC_ML_BAN_FOLDER, psz_folder );
}

static inline int vlc_ml_unban_folder( vlc_medialibrary_t* p_ml, const char* psz_folder )
{
    return p_ml->pf_control( p_ml, VLC_ML_UNBAN_FOLDER, psz_folder );
}

static inline int vlc_ml_list_folder( vlc_medialibrary_t* p_ml,
                                      vlc_ml_entrypoint_t** pp_entrypoints, size_t* p_nb_items )
{
    return p_ml->pf_control( p_ml, VLC_ML_LIST_FOLDERS, pp_entrypoints, p_nb_items );
}

static inline int vlc_ml_pause_background( vlc_medialibrary_t* p_ml )
{
    return p_ml->pf_control( p_ml, VLC_ML_PAUSE_BACKGROUND );
}

static inline int vlc_ml_resume_background( vlc_medialibrary_t* p_ml )
{
    return p_ml->pf_control( p_ml, VLC_ML_RESUME_BACKGROUND );
}

static inline int vlc_ml_clear_history( vlc_medialibrary_t* p_ml )
{
    return p_ml->pf_control( p_ml, VLC_ML_CLEAR_HISTORY );
}

static inline vlc_ml_media_t* vlc_ml_new_external_media( vlc_medialibrary_t* p_ml, const char* psz_mrl )
{
    vlc_ml_media_t* res;
    if ( p_ml->pf_control( p_ml, VLC_ML_NEW_EXTERNAL_MEDIA, psz_mrl, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline vlc_ml_media_t* vlc_ml_new_stream( vlc_medialibrary_t* p_ml, const char* psz_mrl )
{
    vlc_ml_media_t* res;
    if ( p_ml->pf_control( p_ml, VLC_ML_NEW_STREAM, psz_mrl, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline int vlc_ml_media_increase_playcount( vlc_medialibrary_t* p_ml, int64_t i_media_id )
{
    return p_ml->pf_control( p_ml, VLC_ML_MEDIA_INCREASE_PLAY_COUNT, i_media_id );
}

static inline int vlc_ml_media_get_playback_pref( vlc_medialibrary_t* p_ml, int64_t i_media_id, int i_pref, char** ppsz_result )
{
    return p_ml->pf_control( p_ml, VLC_ML_MEDIA_GET_MEDIA_PLAYBACK_PREF, i_media_id, i_pref, ppsz_result );
}

static inline int vlc_ml_media_set_playback_pref( vlc_medialibrary_t* p_ml, int64_t i_media_id, int i_pref, const char* psz_value )
{
    return p_ml->pf_control( p_ml, VLC_ML_MEDIA_SET_MEDIA_PLAYBACK_PREF, i_media_id, i_pref, psz_value );
}

static inline int vlc_ml_media_set_thumbnail( vlc_medialibrary_t* p_ml, int64_t i_media_id, const char* psz_mrl )
{
    return p_ml->pf_control( p_ml, VLC_ML_MEDIA_SET_THUMBNAIL, i_media_id, psz_mrl );
}

static inline int vlc_ml_media_add_external_mrl( vlc_medialibrary_t* p_ml, int64_t i_media_id,
                                                 const char* psz_mrl, int i_type )
{
    return p_ml->pf_control( p_ml, VLC_ML_MEDIA_ADD_EXTERNAL_MRL, i_media_id, psz_mrl, i_type );
}

static inline vlc_ml_media_t* vlc_ml_get_media( vlc_medialibrary_t* p_ml, int64_t i_media_id )
{
    return (vlc_ml_media_t*)p_ml->pf_get( p_ml, VLC_ML_GET_MEDIA, i_media_id );
}

static inline vlc_ml_album_t* vlc_ml_get_album( vlc_medialibrary_t* p_ml, int64_t i_album_id )
{
    return (vlc_ml_album_t*)p_ml->pf_get( p_ml, VLC_ML_GET_ALBUM, i_album_id );
}

static inline vlc_ml_artist_t* vlc_ml_get_artist( vlc_medialibrary_t* p_ml, int64_t i_artist_id )
{
    return (vlc_ml_artist_t*)p_ml->pf_get( p_ml, VLC_ML_GET_ARTIST, i_artist_id );
}

static inline vlc_ml_genre_t* vlc_ml_get_genre( vlc_medialibrary_t* p_ml, int64_t i_genre_id )
{
    return (vlc_ml_genre_t*)p_ml->pf_get( p_ml, VLC_ML_GET_GENRE, i_genre_id );
}

static inline vlc_ml_show_t* vlc_ml_get_show( vlc_medialibrary_t* p_ml, int64_t i_show_id )
{
    return (vlc_ml_show_t*)p_ml->pf_get( p_ml, VLC_ML_GET_SHOW, i_show_id );
}

static inline vlc_ml_playlist_t* vlc_ml_get_playlist( vlc_medialibrary_t* p_ml, int64_t i_playlist_id )
{
    return (vlc_ml_playlist_t*)p_ml->pf_get( p_ml, VLC_ML_GET_PLAYLIST, i_playlist_id );
}

static inline vlc_ml_media_list_t* vlc_ml_list_media_of( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int i_parent_type, int64_t i_parent_id )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_media_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_MEDIA_OF, params, i_parent_type, i_parent_id, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_media_of( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int i_parent_type, int64_t i_parent_id )
{
    vlc_assert( p_ml != NULL );
    size_t res;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_MEDIA_OF, params, i_parent_type, i_parent_id, &res ) )
        return 0;
    return res;
}

static inline vlc_ml_artist_list_t* vlc_ml_list_artist_of( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int i_parent_type, int64_t i_parent_id )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_artist_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_ARTISTS_OF, params, i_parent_type, i_parent_id, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_artists_of( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int i_parent_type, int64_t i_parent_id )
{
    vlc_assert( p_ml != NULL );
    size_t res;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_ARTISTS_OF, params, i_parent_type, i_parent_id, &res ) )
        return 0;
    return res;
}

static inline vlc_ml_album_list_t* vlc_ml_list_albums_of( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int i_parent_type, int64_t i_parent_id )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_album_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_ALBUMS_OF, params, i_parent_type, i_parent_id, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_albums_of( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int i_parent_type, int64_t i_parent_id )
{
    vlc_assert( p_ml != NULL );
    size_t res;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_ALBUMS_OF, params, i_parent_type, i_parent_id, &res ) )
        return 0;
    return res;
}

static inline vlc_ml_media_list_t* vlc_ml_list_album_tracks( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_album_id )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_media_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_ALBUM_TRACKS, params, i_album_id, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_album_tracks( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_album_id )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_ALBUM_TRACKS, params, i_album_id, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_media_list_t* vlc_ml_list_album_artists( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_album_id )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_media_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_ALBUM_ARTISTS, params, i_album_id, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_album_artists( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_album_id )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_ALBUM_ARTISTS, params, i_album_id, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_album_list_t* vlc_ml_list_artist_albums( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_artist_id )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_album_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_ARTIST_ALBUMS, params, i_artist_id, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_artist_albums( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_artist_id )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_ARTIST_ALBUMS, params, i_artist_id, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_media_list_t* vlc_ml_list_artist_tracks( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_artist_id )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_media_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_ARTIST_TRACKS, params, i_artist_id, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_artist_tracks( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_artist_id )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_ARTIST_TRACKS, params, i_artist_id, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_media_list_t* vlc_ml_list_video_media( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_media_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_VIDEOS, params, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_video_media( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_VIDEOS, params, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_media_list_t* vlc_ml_list_audio_media( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_media_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_AUDIOS, params, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_audio_media( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_AUDIOS, params, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_album_list_t* vlc_ml_list_albums( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_album_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_ALBUMS, params, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_albums( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_ALBUMS, params, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_genre_list_t* vlc_ml_list_genres( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_genre_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_GENRES, params, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_genres( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_GENRES, params, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

/**
 * @brief vlc_ml_list_artists
 * @param params Query parameters, or NULL for the default
 * @param b_include_all True if you wish to fetch artists without at least one album.
 * @return
 */
static inline vlc_ml_artist_list_t* vlc_ml_list_artists( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, bool b_include_all )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_artist_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_ARTISTS, params, (int)b_include_all, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_artists( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, bool includeAll )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_ARTISTS, params, includeAll, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_show_list_t* vlc_ml_list_shows( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_show_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_SHOWS, params, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_shows( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_SHOWS, params, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_media_list_t* vlc_ml_list_genre_artists( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_genre_id )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_media_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_GENRE_ARTISTS, params, i_genre_id, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_genre_artists( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_genre_id )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_GENRE_ARTISTS, params, i_genre_id, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_media_list_t* vlc_ml_list_genre_tracks( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_genre_id )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_media_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_GENRE_TRACKS, params, i_genre_id, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_genre_tracks( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_genre_id )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_GENRE_TRACKS, params, i_genre_id, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_album_list_t* vlc_ml_list_genre_albums( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_genre_id )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_album_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_GENRE_ALBUMS, params, i_genre_id, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_genre_albums( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_genre_id )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_GENRE_ALBUMS, params, i_genre_id, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_media_list_t* vlc_ml_list_show_episodes( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_show_id )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_media_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_SHOW_EPISODES, params, i_show_id, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_show_episodes( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_show_id )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_GENRE_ALBUMS, params, i_show_id, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_label_list_t* vlc_ml_list_media_labels( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_media_id )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_label_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_MEDIA_LABELS, params, i_media_id, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_media_labels( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params, int64_t i_media_id )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_MEDIA_LABELS, params, i_media_id, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

static inline vlc_ml_media_list_t* vlc_ml_list_history( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_media_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_HISTORY, params, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline vlc_ml_media_list_t* vlc_ml_list_stream_history( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_media_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_STREAM_HISTORY, params, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline vlc_ml_playlist_list_t* vlc_ml_list_playlists( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    vlc_ml_playlist_list_t* res;
    if ( p_ml->pf_list( p_ml, VLC_ML_LIST_PLAYLISTS, params, &res ) != VLC_SUCCESS )
        return NULL;
    return res;
}

static inline size_t vlc_ml_count_playlists( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params )
{
    vlc_assert( p_ml != NULL );
    size_t count;
    if ( p_ml->pf_list( p_ml, VLC_ML_COUNT_PLAYLISTS, params, &count ) != VLC_SUCCESS )
        return 0;
    return count;
}

#ifdef __cplusplus
}
#endif /* C++ */

#ifndef __cplusplus
# define vlc_ml_release( OBJ ) _Generic( ( OBJ ), \
    vlc_ml_show_t*: vlc_ml_show_release, \
    vlc_ml_artist_t*: vlc_ml_artist_release, \
    vlc_ml_album_t*: vlc_ml_album_release, \
    vlc_ml_genre_t*: vlc_ml_genre_release, \
    vlc_ml_media_t*: vlc_ml_media_release, \
    vlc_ml_playlist_t*: vlc_ml_playlist_release, \
    vlc_ml_label_list_t*: vlc_ml_label_list_release, \
    vlc_ml_file_list_t*: vlc_ml_file_list_release, \
    vlc_ml_artist_list_t*: vlc_ml_artist_list_release, \
    vlc_ml_media_list_t*: vlc_ml_media_list_release, \
    vlc_ml_album_list_t*: vlc_ml_album_list_release, \
    vlc_ml_show_list_t*: vlc_ml_show_list_release, \
    vlc_ml_genre_list_t*: vlc_ml_genre_list_release, \
    vlc_ml_playlist_list_t*: vlc_ml_playlist_list_release \
    )( OBJ )
#else
static inline void vlc_ml_release( vlc_ml_show_t* show ) { vlc_ml_show_release( show ); }
static inline void vlc_ml_release( vlc_ml_artist_t* artist ) { vlc_ml_artist_release( artist ); }
static inline void vlc_ml_release( vlc_ml_album_t* album ) { vlc_ml_album_release( album ); }
static inline void vlc_ml_release( vlc_ml_genre_t* genre ) { vlc_ml_genre_release( genre ); }
static inline void vlc_ml_release( vlc_ml_media_t* media ) { vlc_ml_media_release( media ); }
static inline void vlc_ml_release( vlc_ml_playlist_t* playlist ) { vlc_ml_playlist_release( playlist ); }
static inline void vlc_ml_release( vlc_ml_label_list_t* list ) { vlc_ml_label_list_release( list ); }
static inline void vlc_ml_release( vlc_ml_file_list_t* list ) { vlc_ml_file_list_release( list ); }
static inline void vlc_ml_release( vlc_ml_artist_list_t* list ) { vlc_ml_artist_list_release( list ); }
static inline void vlc_ml_release( vlc_ml_media_list_t* list ) { vlc_ml_media_list_release( list ); }
static inline void vlc_ml_release( vlc_ml_album_list_t* list ) { vlc_ml_album_list_release( list ); }
static inline void vlc_ml_release( vlc_ml_show_list_t* list ) { vlc_ml_show_list_release( list ); }
static inline void vlc_ml_release( vlc_ml_genre_list_t* list ) { vlc_ml_genre_list_release( list ); }
static inline void vlc_ml_release( vlc_ml_playlist_list_t* list ) { vlc_ml_playlist_list_release( list ); }
#endif

#endif /* VLC_MEDIA_LIBRARY_H */

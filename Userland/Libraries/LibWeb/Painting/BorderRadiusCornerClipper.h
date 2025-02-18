/*
 * Copyright (c) 2022, MacDue <macdue@dueutil.tech>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibGfx/AntiAliasingPainter.h>
#include <LibWeb/Painting/BorderPainting.h>

namespace Web::Painting {

enum class CornerClip {
    Outside,
    Inside
};

class BorderRadiusCornerClipper : public RefCounted<BorderRadiusCornerClipper> {
public:
    static ErrorOr<NonnullRefPtr<BorderRadiusCornerClipper>> create(CornerRadii const&, DevicePixelRect const& border_rect, BorderRadiiData const& border_radii, CornerClip corner_clip = CornerClip::Outside);

    void sample_under_corners(Gfx::Painter& page_painter);
    void blit_corner_clipping(Gfx::Painter& page_painter);

    struct CornerData {
        CornerRadii corner_radii;
        struct CornerLocations {
            DevicePixelPoint top_left;
            DevicePixelPoint top_right;
            DevicePixelPoint bottom_right;
            DevicePixelPoint bottom_left;
        };
        CornerLocations page_locations;
        CornerLocations bitmap_locations;
    } m_data;

    DevicePixelRect border_rect() const { return m_border_rect; }

    BorderRadiusCornerClipper(CornerData corner_data, NonnullRefPtr<Gfx::Bitmap> corner_bitmap, CornerClip corner_clip, DevicePixelRect const& border_rect)
        : m_data(move(corner_data))
        , m_corner_bitmap(corner_bitmap)
        , m_corner_clip(corner_clip)
        , m_border_rect(border_rect)
    {
    }

private:
    NonnullRefPtr<Gfx::Bitmap> m_corner_bitmap;
    bool m_has_sampled { false };
    CornerClip m_corner_clip { false };
    DevicePixelRect m_border_rect;
};

struct ScopedCornerRadiusClip {
    ScopedCornerRadiusClip(PaintContext& context, DevicePixelRect const& border_rect, BorderRadiiData const& border_radii, CornerClip corner_clip = CornerClip::Outside);

    ~ScopedCornerRadiusClip();

    AK_MAKE_NONMOVABLE(ScopedCornerRadiusClip);
    AK_MAKE_NONCOPYABLE(ScopedCornerRadiusClip);

private:
    PaintContext& m_context;
    RefPtr<BorderRadiusCornerClipper> m_corner_clipper;
};

}
